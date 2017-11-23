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
        MotorSimDataPoint* newDataPoint = new MotorSimDataPoint;
        OpenBurnMotor* newDataPointMotor = new OpenBurnMotor;
        newDataPointMotor->SetNozzle(m_InitialDesignMotor->GetNozzle());

        if (iterations == 0) //start with initial conditions
        {
            newDataPointMotor->SetGrains(m_InitialDesignMotor->GetGrains(), true);
        }
        else if (iterations > 0 && !m_SimResultData.empty())
        {
            //after we've run the sim for one time step, use the previous result as initial condition
            newDataPointMotor->SetGrains(m_SimResultData[iterations-1]->motor->GetGrains(), true);
        }
        m_TotalBurnTime += settings->timeStep;
        newDataPoint->time = m_TotalBurnTime;
        newDataPoint->motor = newDataPointMotor;        
        newDataPoint->pressure = CalcChamberPressure(newDataPoint->motor);
        newDataPoint->thrust = CalcThrust(newDataPoint->motor, settings, newDataPoint->pressure);
        m_TotalImpulse += (newDataPoint->thrust * settings->timeStep);
        for (auto* i : newDataPointMotor->GetGrains())
        {
            newDataPoint->burnRate = CalcSteadyStateBurnRate(newDataPointMotor, i);
            i->SetBurnRate(newDataPoint->burnRate);

            //OpenBurnGrain::Burn should return false if the grain burned out
            if (!i->GetIsBurnedOut() && !i->Burn(settings->timeStep))
            {
                numBurnedOut++;
            }
        }
        newDataPoint->massflux = CalcCoreMassFlux(newDataPoint->motor);

        m_SimResultData.push_back(newDataPoint);
        if (iterations > int(1e5))
        {
            emit SimulationFinished(false);            
            break;
        }
        iterations++;
    }
    emit SimulationFinished(true);
}

//mdot A.K.A Mass flux at given motor X value
double MotorSim::CalcMassFlux(OpenBurnMotor* motor, double xVal)
{
    OpenBurnGrain* grain = motor->GetGrainAtX(xVal);
    if (grain)
    {
        return motor->GetUpstreamMassFlow(xVal) / grain->GetPortArea();
    }
    return 0;
}
double MotorSim::CalcMachNumber(OpenBurnMotor* motor, double xVal, double massFlux)
{
    //mdot = M * A * P_0 * sqrt(k / R * T_0) * (1 + (k - 1) / 2 * M^2) ^ - (k + 1) / (2(k-1))
    //P_0 = gas pressure, T_0 = gas temperature, k = ratio specific heats (cp/cv), M = mach number, R = gas constant
    //see https://spaceflightsystems.grc.nasa.gov/education/rocket/mflchk.html
    //and http://propulsion-skrishnan.com/pdf/Erosive%20Burning%20of%20Solid%20Propellants.pdf
    return 0;
}
//bottom of the last grain
double MotorSim::CalcCoreMassFlux(OpenBurnMotor* motor)
{
    return CalcMassFlux(motor, 0);
}
double MotorSim::CalcCoreMachNumber(OpenBurnMotor* motor, double coreMassFlux)
{
    return CalcMachNumber(motor, 0, coreMassFlux);
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
        return 1.f;
    }
    else
    {
        machNumber = 2.2f; //supersonic
    }
    double gamma = motor->GetAvgPropellant().GetSpecificHeatRatio();
    double areaRatio = motor->GetNozzle()->GetNozzleExpansionRatio();
    double gp1 = gamma + 1.f; //gamma plus 1
    double gm1 = gamma -1.f ; //gamma minus 1

    double deriv = 0, areaRatioNew = 0, fac = 0;
    double exponent = gp1 / (2.f * gm1) ;

    double areaRatioGuess = areaRatio / 2.f; //initial guess
    double machGuess = machNumber + .05f;
    while (qAbs(areaRatio - areaRatioGuess) > .0001f)
    {
       fac = 1.f + 0.5f * gm1 * machGuess * machGuess;
       areaRatioNew = 1.f / (machGuess * qPow(fac, -exponent) * qPow(gp1/2.f, exponent));
       deriv = (areaRatioNew - areaRatioGuess)/(machGuess-machNumber);
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
    double gm1 = gamma - 1;
    double pRatio = qPow( (1.f + .5f *gm1 * exitMach * exitMach), -(gamma / gm1)); //exit pressure over chamber pressure
    return chamberPressure * pRatio;
}
double MotorSim::CalcChamberPressure(OpenBurnMotor* motor)
{
    //p = (Kn * a * rho * C* )^(1/(1-n))
    double rho = OpenBurnUnits::ConvertMass(
        OpenBurnUnits::MassUnits_T::pounds_mass,
        OpenBurnUnits::MassUnits_T::slugs,
        motor->GetAvgPropellant().GetDensity()); //propellant density
    double Cstar = motor->GetAvgPropellant().GetCharVelocity();
    double exponent = 1.0f / (1.0f - motor->GetAvgPropellant().GetBurnRateExp());
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
    OpenBurnNozzle* nozzle =  motor->GetNozzle();
    double idealCoef = CalcIdealThrustCoefficient(motor, settings, chamberPressure);
    double divergenceLoss = nozzle->GetNozzleDivergenceLossFactor();
    double skinFrictionLoss = (1.f - settings->skinFrictionEfficency);
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
    double kSquareTerm = (2.f * k * k) / (k - 1.f);
    double twoOverKTerm = 2.f / (k + 1.f);
    double kOverItselfTerm = (k + 1.f) / (k - 1.f);
    double kMinusOne = (k - 1.f) / k;

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
    double rho = OpenBurnUnits::ConvertMass(
        OpenBurnUnits::MassUnits_T::pounds_mass, 
        OpenBurnUnits::MassUnits_T::slugs,
        prop.GetDensity()); //propellant density
    double C_p = prop.GetSpecificHeatConstantPressure(); //specific heat of combustion products (gas at constant pressure)
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
std::vector<MotorSimDataPoint*>& MotorSim::GetResults()
{
    return m_SimResultData;
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
    for (auto i : m_SimResultData)
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
    for (auto i : m_SimResultData)
    {
        if (i->massflux > maxMassFlux)
            maxMassFlux = i->massflux;
    }
    return maxMassFlux;
}
double MotorSim::GetAvgThrust() const
{
    return m_TotalImpulse / m_TotalBurnTime;
}
void MotorSim::ClearAllData()
{
    for (auto& i : m_SimResultData)
    {
        delete i;
    }
    m_SimResultData.clear();
}
void MotorSim::SetDesignMotor(OpenBurnMotor *motor)
{
    m_InitialDesignMotor = motor;
}
