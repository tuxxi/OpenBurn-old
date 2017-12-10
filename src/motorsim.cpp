#include "motorsim.h"
#include "units.h"

#include <QtDebug>

using OpenBurnUtil::g_kGasConstantR;

MotorSim::MotorSim() 
    : MotorSim(nullptr)
{

}
MotorSim::MotorSim(OpenBurnMotor* motor)
    : m_InitialDesignMotor(motor),
    m_TotalBurnTime(0),
    m_TotalImpulse(0)
{
    ClearAllData();
}
MotorSim::~MotorSim()
{
    ClearAllData();
}
void MotorSim::RunSim(MotorSimSettings* settings)
{
    ClearAllData();
    
    int iterations = 0;
    size_t numBurnedOut = 0;
    
    m_TotalBurnTime = 0;
    m_TotalImpulse = 0;
    emit SimulationStarted();
    while(numBurnedOut < m_InitialDesignMotor->GetNumGrains())
    {
        auto newDataPoint = std::make_unique<MotorSimDataPoint>();
        auto newDataPointMotor = std::make_unique<OpenBurnMotor>();

        //we need to clone the motor's data so that we can modify it for eacn time slice during the simulation
        newDataPointMotor->SetNozzle(std::move(m_InitialDesignMotor->GetNozzle()->Clone())); //clone nozzle ptr

        //clone grain ptrs
        if (iterations == 0) //start with initial conditions
        {
            newDataPointMotor->SetGrains(m_InitialDesignMotor->GetGrains(), true);
        }
        else if (iterations > 0 && !m_SimResultData.empty())
        {
            //after we've run the sim for one time step, use the previous result as initial condition
            newDataPointMotor->SetGrains(m_SimResultData[iterations-1]->motor->GetGrains(), true);
        }
        newDataPoint->motor = std::move(newDataPointMotor);
        newDataPoint->pressure = CalcChamberPressure(newDataPoint->motor.get());
        newDataPoint->time = m_TotalBurnTime;

        for (auto& i : newDataPoint->motor->GetGrains())
        {
            newDataPoint->burnRate = CalcSteadyStateBurnRate(newDataPoint->motor.get(), i.get());
            i->SetBurnRate(newDataPoint->burnRate);

            //OpenBurnGrain::Burn should return false if the grain burned out
            if (!i->GetIsBurnedOut() && !i->Burn(settings->timeStep))
            {
                numBurnedOut++;
            }
        }
        newDataPoint->thrust = CalcThrust(newDataPoint->motor.get(), settings, newDataPoint->pressure);
        newDataPoint->massflux = CalcCoreMassFlux(newDataPoint->motor.get());
        newDataPoint->isp = CalcIsp(newDataPoint->motor.get(), newDataPoint->thrust);
        m_TotalImpulse += (newDataPoint->thrust * settings->timeStep);
        m_TotalBurnTime += settings->timeStep;

        m_SimResultData.push_back(std::move(newDataPoint));
        if (m_TotalBurnTime > 1000.0) //failure state - 1000 second burn time
        {
            emit SimulationFinished(false);
            break;
        }
        iterations++;
    }
    emit SimulationFinished(true);
}

//mdot A.K.A Mass flux at given motor X value
//x = 0 at motor head end 
double MotorSim::CalcMassFlux(OpenBurnMotor* motor, double xVal)
{
    OpenBurnGrain* grain = motor->GetGrainAtX(xVal);
    if (grain)
    {
        return motor->GetUpstreamMassFlow(xVal) / grain->GetPortArea();
    }
    return 0; //grain not found!
}
//x = 0 at fwd end
double MotorSim::CalcMachNumber(OpenBurnMotor* motor, double xVal)
{
    //mdot = M * P_0 * sqrt(k / R * T_0) * (1 + (k - 1) / 2 * M^2) ^ - (k + 1) / (2(k-1))
    //P_0 = gas pressure, T_0 = gas temperature, k = ratio specific heats (cp/cv), M = mach number, R = gas constant
    //see https://spaceflightsystems.grc.nasa.gov/education/rocket/mflchk.html
    //and http://propulsion-skrishnan.com/pdf/Erosive%20Burning%20of%20Solid%20Propellants.pdf
    const OpenBurnPropellant& prop = motor->GetAvgPropellant();
    double gamma = prop.GetSpecificHeatRatio();
    double gm1 = gamma - 1;
    double gp1 = gamma + 1;

    double R = prop.GetCp() - prop.GetCv();
    double SqrtTt = qSqrt(CalcTotalTemperature(motor));
    double Pt = CalcChamberPressure(motor);
    double massFlux = CalcMassFlux(motor, xVal);
    double massFluxGuess = massFlux / 2.0;
    double deriv = 0, massFluxNew = 0, machNumber = 0, machGuess = 0;

    while (qAbs(massFlux - massFluxGuess) > .0001)
    {
        //run through ideal compressable gas eq with our mach guess to find mass flux
        double coef = machGuess * (Pt / SqrtTt) * qSqrt(gamma / R);
        double base = 1.0 + gm1/2.0 * machGuess * machGuess;
        massFluxNew = coef * qPow(base, -(gp1 / (2.0*gm1) ));

        //find change
        deriv = (massFluxNew - massFluxGuess) / (machGuess - machNumber);

        //update guesses
        massFluxGuess = massFluxNew;
        machNumber = machGuess;
        machGuess = machNumber + (massFlux - massFluxGuess) / deriv;
    }
    return machNumber;
}
double MotorSim::CalcTotalTemperature(OpenBurnMotor* motor)
{
    //Tt = Tc/ (1 + k-1 / 2)
    //http://www.braeunig.us/space/propuls.htm
    double Tc = motor->GetAvgPropellant().GetAdiabaticFlameTemp();
    double gmma = motor->GetAvgPropellant().GetSpecificHeatRatio();
    return Tc / (1.0 + ((gmma - 1.0) / 2.0));
}
//bottom of the last grain
double MotorSim::CalcCoreMassFlux(OpenBurnMotor* motor)
{
    return CalcMassFlux(motor, motor->GetMotorLength());
}
double MotorSim::CalcCoreMachNumber(OpenBurnMotor* motor)
{
    return CalcMachNumber(motor, motor->GetMotorLength());
}
double MotorSim::CalcIsp(OpenBurnMotor* motor, double thrust)
{
    //Isp = F / mdot * g
    return thrust /  motor->GetTotalMassFlow();
}

//Calculates the exit mach number from the area ratio.
//https://www.grc.nasa.gov/www/k-12/airplane/rktthsum.html
double MotorSim::CalcExitMachNumber(OpenBurnMotor* motor)
{
    //Ae/A* = ((gamma + 1) / 2) ^ exponent * (1 + gamma
    //where exponent = (gamma +1) / 2(gamma - 1)
    double machNumber = 0;
    if (motor->GetNozzle()->GetNozzleExit() <= motor->GetNozzle()->GetNozzleThroat()) //sonic nozzle
    {
        return 1.0;
    }
    else
    {
        machNumber = 2.2; //supersonic
    }
    double gamma = motor->GetAvgPropellant().GetSpecificHeatRatio();
    double areaRatio = motor->GetNozzle()->GetNozzleExpansionRatio();
    double gp1 = gamma + 1.0;
    double gm1 = gamma -1.0 ;

    double deriv = 0, areaRatioNew = 0, fac = 0;
    double exponent = gp1 / (2. * gm1) ;

    double areaRatioGuess = areaRatio / 2.0; //initial guess
    double machGuess = machNumber + .05;
    while (qAbs(areaRatio - areaRatioGuess) > .0001)
    {
        //find area ratio with our guess
        fac = 1.0 + 0.5 * gm1 * machGuess * machGuess;
        areaRatioNew = 1.0 / (machGuess * qPow(fac, -exponent) * qPow(gp1/2.0, exponent));

        //find change (deriv)
        deriv = (areaRatioNew - areaRatioGuess)/(machGuess-machNumber);

        //update guesses
        areaRatioGuess = areaRatioNew;
        machNumber = machGuess;
        machGuess = machNumber + (areaRatio - areaRatioGuess)/deriv;
    }
    return machNumber;
}
//Calculates the nozzle exit pressure given an exit mach number and chamber pressure
double MotorSim::CalcExitPressure(OpenBurnMotor* motor, double chamberPressure, double exitMach)
{
    double gamma = motor->GetAvgPropellant().GetSpecificHeatRatio();
    double gm1 = gamma - 1.0;
    double pRatio = qPow( (1.0 + .5 *gm1 * exitMach * exitMach), -(gamma / gm1)); //exit pressure over chamber pressure
    return chamberPressure * pRatio;
}
double MotorSim::CalcChamberPressure(OpenBurnMotor* motor)
{
    //p = (Kn * a * rho * C* )^(1/(1-n))
    double rho = OpenBurnUnits::MassUnits::Convert(
        OpenBurnUnits::MassUnits_T::pounds_mass,
        OpenBurnUnits::MassUnits_T::slugs,
        motor->GetAvgPropellant().GetDensity()); //propellant density
    double Cstar = motor->GetAvgPropellant().GetCharVelocity();
    double exponent = 1.0 / (1.0 - motor->GetAvgPropellant().GetBurnRateExp());
    double p1 = motor->CalcKn() * motor->GetAvgPropellant().GetBurnRateCoef() * rho * Cstar;
    return qPow(p1, exponent);
}
//Calculates the actual thrust coefficent using some emperical inefficency factors,
//then applys that Cf to the throat area and pressure to give the real thrust value
double MotorSim::CalcThrust(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure)
{
    //http://www.dtic.mil/dtic/tr/fulltext/u2/a099791.pdf
    //REAL thrust coeff = Nd * Nt * (Nf * Cfv + (1 - nf))
    //Nd is divergence loss, 
    //Nd is two phase flow loss
    //Nf is skin friction loss
    //Cfv is the ideal thrust coefficent. Note that the reference text includes a correction
    //for ambient pressure, this is already done by the ideal thrust coefficent calculation.
    const auto nozzle =  motor->GetNozzle();
    double idealCoef = CalcIdealThrustCoefficient(motor, settings, chamberPressure);
    double divergenceLoss = nozzle->GetNozzleDivergenceLossFactor();
    double skinFrictionLoss = (1.0 - settings->skinFrictionEfficency);
    double realCoef = (divergenceLoss * settings->twoPhaseFlowEfficency *
        (settings->skinFrictionEfficency * idealCoef + skinFrictionLoss));
    
    return realCoef * nozzle->GetNozzleThroatArea() * chamberPressure;
}
//Calculates the ideal thrust coefficent given an operating pressure
double MotorSim::CalcIdealThrustCoefficient(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure)
{
    //F = At * P1 * sqrt [ (2k^2 / k-1) (2/k+1 )^k+1k-1 * 1- (P2/P1)^(k-1/k) )] + (p2-p3)A2
    //See Rocket Propulsion Elements, Eq. 3-29
    //A lot of this equation deals with k, so we'll do our best to simplify those first
    double k = motor->GetAvgPropellant().GetSpecificHeatRatio();
    double kSquareTerm = (2.0 * k * k) / (k - 1.0);
    double twoOverKTerm = 2.0 / (k + 1.0);
    double kOverItselfTerm = (k + 1.0) / (k - 1.0);
    double kMinusOne = (k - 1.0) / k;

    double exitMach = CalcExitMachNumber(motor);
    double exitPressure = CalcExitPressure(motor, chamberPressure, exitMach);
    double pRatio = exitPressure / chamberPressure;
    double momentumThrust = qSqrt(kSquareTerm * qPow(twoOverKTerm,kOverItselfTerm) * (1.f - qPow(pRatio, kMinusOne)));
    double pressureThrust = ((exitPressure - settings->ambientPressure) * motor->GetNozzle()->GetNozzleExpansionRatio() ) / chamberPressure;
    return momentumThrust + pressureThrust;
}
double MotorSim::CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain)
{
    // r = a * p^n
    OpenBurnPropellant prop = grain->GetPropellantType();
    return prop.GetBurnRateCoef() * qPow(CalcChamberPressure(motor), prop.GetBurnRateExp());
}
double MotorSim::CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, MotorSimSettings* settings, double machNumber)
{
    OpenBurnPropellant prop = grain->GetPropellantType();
    double k = prop.GetSpecificHeatRatio();
    double P_s = CalcChamberPressure(motor); //steady-state chamber pressure

    //First we have to calculate saint roberts law again, using static pressure at grain surface obtained by:
    //P = P_0(1+ (k-1)/2 * M^2) ^ -k / (k-1)
    double base = 1.0f + (k - 1.0f)/2.0f * machNumber * machNumber;
    double exponent = -1.0f*k / (k - 1.0f);
    double P = P_s * qPow(base, exponent);

    //This burn rate value is __lower__ than saint robert's law due to stagnation pressure considerations.
    //Normally this value would be used as R_0 but the core mach number is not considered in the basic case,
    //So we calculate it here and subtract the difference from R_e at the end.
    double R_0 = prop.GetBurnRateCoef() * qPow(P, prop.GetBurnRateExp());
    double R_diff = CalcSteadyStateBurnRate(motor, grain) - R_0; //this factor will be removed later on

    //For these calculations we need LOTS of new variables!
    double beta =  53; //(experimental, chosen by Lenoir and Robillard)
    double G = CalcMassFlux(motor, grain->GetPortArea()); // mass flux
    double D = grain->GetHydraulicDiameter(); // = hydraulic diameter, 4* area / perimeter
    double C_s = prop.GetPropellantSpecificHeat(); // specific heat of propellant (NOT combustion gas)
    double rho = OpenBurnUnits::MassUnits::Convert(
        OpenBurnUnits::MassUnits_T::pounds_mass,
        OpenBurnUnits::MassUnits_T::slugs,
        prop.GetDensity()); //propellant density
    double C_p = prop.GetCp(); //specific heat of combustion products (gas at constant pressure)
    double T_0 = prop.GetAdiabaticFlameTemp();// adiabatic flame temperature
    double mu = prop.GetGasViscosity(); //viscoisty of combustion products
    double Pr = prop.GetPrandtlNumber(); //prandtl number

    double T_s = T_0 / 2.0f;            //Temperature of burning propellant surface
    double T_i = settings->ambientTemp; //Base temperature of propellant (degrees K)
    /*
    Now we calculate erosive burning from the given equation:
    R_e = (alpha * G^0.8 ) / [D^0.2 * e^( (beta * R_ttl * rho)/G )]
    where
    alpha = (0.0288 * Cp * mu^0.2 * Pr^-2/3) / (Rho * Cs) * (T_0 - T_s) / (T_s - T_i)
    see http://propulsion-skrishnan.com/pdf/Erosive%20Burning%20of%20Solid%20Propellants.pdf
    and Rocket Propulsion Elements by G P. Sutton
    */
    double alpha =(0.0288f * C_p * qPow(mu, 0.2) * qPow(Pr, -2.0f/3.0f ) / (rho * C_s));
    alpha = alpha * ( (T_0 - T_s) / (T_s - T_i) );

    //we turn the burn rate equation into this closed form in order to solve for r
    //f(r) = R_0 + a*e^(b*r) - r
    double a = (alpha * qPow(G, 0.8) ) * qPow(D, -0.2f);
    double b = -1.0f*(beta *rho)/G;

    // use newtons method to solve iteratively:
    double R_total = R_0; //start at steady state conditions
    for (int i = 0; i < OpenBurnUtil::g_kNumInterations; i++) //
    {
        //d/dx ae^(br)-r = abe^(br)-1
        R_total = R_total - (R_total + a * qPow(M_E, b * R_total) - R_total) / (R_total + a * b * qPow(M_E, b * R_total) - 1.0f);
    }
    double R_e = R_total - R_0 - R_diff;
    return R_e;
}

double MotorSim::GetTotalBurnTime() const
{
    return m_TotalBurnTime;
}
double MotorSim::GetTotalImpulse() const
{
    return m_TotalImpulse;
}
double MotorSim::GetMaxPressure() const
{
    double maxPressure = 0;
    for (auto& i : m_SimResultData)
    {
        if (i->pressure > maxPressure)
        {
            maxPressure = i->pressure;
        }
    }
    return maxPressure;
}
double MotorSim::GetMaxMassFlux() const
{
    double maxMassFlux = 0;
    for (auto& i : m_SimResultData)
    {
        if (i->massflux > maxMassFlux)
            maxMassFlux = i->massflux;
    }
    return maxMassFlux;
}

QString MotorSim::GetMotorDesignation() const
{
    const double totalImpulseN = OpenBurnUnits::ForceUnits::Convert(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        GetTotalImpulse());

    const double avgThrustN = OpenBurnUnits::ForceUnits::Convert(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        GetAvgThrust());

    const QString designation(OpenBurnUtil::GetMotorClass(totalImpulseN));
    const QString thrust(QString::number(round(avgThrustN)));
    return QString(designation + "-" + thrust);
}

QString MotorSim::GetFullMotorDesignation() const
{
    const double totalImpulseN = OpenBurnUnits::ForceUnits::Convert(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        GetTotalImpulse());

    const QString percent(QString::number(OpenBurnUtil::GetMotorClassPercent(totalImpulseN), 'g', 2));
    return QString(percent + "% " + GetMotorDesignation());
}

std::vector<std::unique_ptr<MotorSimDataPoint>>::iterator MotorSim::GetResultsBegin()
{
    return m_SimResultData.begin();
}
std::vector<std::unique_ptr<MotorSimDataPoint>>::iterator MotorSim::GetResultsEnd()
{
    return m_SimResultData.end();
}
MotorSimDataPoint* MotorSim::GetResult(int index)
{
    if (GetResultsEmpty())
        return nullptr;
    return m_SimResultData[index].get();
}

size_t MotorSim::GetNumPoints() const
{
    return m_SimResultData.size();
}

OpenBurnMotor* MotorSim::GetDesignMotor() const
{
    return m_InitialDesignMotor;
}
bool MotorSim::GetResultsEmpty() const
{
    return qFuzzyIsNull(m_TotalBurnTime) || m_SimResultData.empty();
}
double MotorSim::GetAvgThrust() const
{
    return m_TotalImpulse / m_TotalBurnTime;
}
void MotorSim::ClearAllData()
{
    m_SimResultData.clear();
}
void MotorSim::SetDesignMotor(OpenBurnMotor *motor)
{
    m_InitialDesignMotor = motor;
}
