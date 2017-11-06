#include "motorsim.h"
#include "util.h"
#include <QtDebug>
using OpenBurnUtil::g_kGasConstantR;

MotorSim::MotorSim() 
    : m_InitialDesignMotor(nullptr), m_TotalBurnTime(0.0f)
{

}
MotorSim::MotorSim(OpenBurnMotor* motor)
    : m_InitialDesignMotor(motor), m_TotalBurnTime(0.0f)
    
{

}
MotorSim::~MotorSim()
{
    for (auto* i : m_SimResultData)
    {
        delete i;
    }
    m_SimResultData.clear();
}
void MotorSim::RunSim(double timestep)
{
    ClearAllData();
    
    int iterations = 0;
    size_t numBurnedOut = 0;
    
    m_TotalBurnTime = 0;
    while(numBurnedOut < m_InitialDesignMotor->GetNumGrains())
    {
        MotorSimDataPoint* newDataPoint = new MotorSimDataPoint;
        OpenBurnMotor* newDataPointMotor = new OpenBurnMotor;
        newDataPointMotor->SetNozzle(m_InitialDesignMotor->GetNozzle());

        if (m_SimResultData.empty()) //start with initial conditions
        {
            newDataPointMotor->SetCopyGrains(m_InitialDesignMotor->GetGrains());                        
        }
        else //after we've run the sim for one time step, use the previous result as initial condition
        {
            newDataPointMotor->SetCopyGrains(m_SimResultData[iterations-1]->motor->GetGrains());            
        }
        newDataPoint->motor = newDataPointMotor;
        m_TotalBurnTime += timestep;
        newDataPoint->time = m_TotalBurnTime;
        iterations++;        
        for (auto* i : newDataPointMotor->GetGrains())
        {
            double burnRate = CalcSteadyStateBurnRate(newDataPointMotor, i);
            newDataPoint->burnRate = burnRate;
            i->SetBurnRate(burnRate);            
            newDataPoint->pressure = CalcChamberPressure(newDataPoint->motor);    

            // TODO: if (erosive)
            //i->SetErosiveBurnRate();
            if (!i->Burn(timestep)) //OpenBurnGrain::Burn should return false if the grain burned out
            {
                numBurnedOut++;
                qDebug() << "At time T=" << m_TotalBurnTime << ", "<< numBurnedOut << " grains burned out!";                
            }
        }
        m_SimResultData.push_back(newDataPoint);
    }
}
//mdot A.K.A Mass flux at given crossflow mach number and port area
double MotorSim::CalcMassFlux(OpenBurnMotor* motor, double machNumber, double portArea)
{
    //mdot = M * A * P_0 * sqrt(k / R * T_0) * (1 + (k - 1) / 2 * M^2) ^ - (k + 1) / (2(k-1))
    //P_0 = gas pressure, T_0 = gas temperature, k = ratio specific heats (cp/cv), M = mach number, R = gas constant
    //see https://spaceflightsystems.grc.nasa.gov/education/rocket/mflchk.html
    //and http://propulsion-skrishnan.com/pdf/Erosive%20Burning%20of%20Solid%20Propellants.pdf

    double temp = motor->GetAvgPropellant()->GetAdiabaticFlameTemp();
    double k = motor->GetAvgPropellant()->GetSpecificHeatRatio();
    double A = machNumber * portArea * CalcChamberPressure(motor) * qSqrt( (k / (temp * g_kGasConstantR)) );
    double B = 1.0f + (k - 1) / 2 * machNumber * machNumber;
    double C = (k - 1.0f) / 2;
    return A * qPow(B, -1.0f*C);
}
double MotorSim::CalcChamberPressure(OpenBurnMotor* motor)
{
    //p = (Kn * a * rho * C* )^(1/(1-n))
    double exponent = 1.0f / (1.0f - motor->GetAvgPropellant()->GetBurnRateExp());
    double p1 = motor->CalcKn() * motor->GetAvgPropellant()->GetBurnRateCoef() *
        motor->GetAvgPropellant()->GetDensity() * motor->GetAvgPropellant()->GetCharVelocity();
    double chamberPressure = qPow(p1, exponent);
    return chamberPressure;
}
double MotorSim::CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain)
{
    // r = a * p^n
    OpenBurnPropellant* prop = grain->GetPropellantType();
    return prop->GetBurnRateCoef() * qPow(CalcChamberPressure(motor), prop->GetBurnRateExp());
}
double MotorSim::CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, double machNumber)
{
    OpenBurnPropellant* prop = grain->GetPropellantType();
    double k = prop->GetSpecificHeatRatio();
    double P_s = CalcChamberPressure(motor); //steady-state chamber pressure

    //First we have to calculate saint roberts law again, using static pressure at grain surface obtained by:
    //P = P_0(1+ (k-1)/2 * M^2) ^ -k / (k-1)
    double base = 1.0f + (k - 1.0f)/2.0f * machNumber * machNumber;
    double exponent = -1.0f*k / (k - 1.0f);
    double P = P_s * qPow(base, exponent);

    //This burn rate value is __lower__ than saint robert's law due to stagnation pressure considerations.
    //Normally this value would be used as R_0 but the core mach number is not considered in the basic case,
    //So we calculate it here and subtract the difference from R_e at the end.
    double R_0 = prop->GetBurnRateCoef() * qPow(P, prop->GetBurnRateExp());
    double R_diff = CalcSteadyStateBurnRate(motor, grain) - R_0; //this factor will be removed later on

    //For these calculations we need LOTS of new variables!
    double beta =  53; //(experimental, chosen by Lenoir and Robillard)
    double G = CalcMassFlux(motor, machNumber, grain->GetPortArea()); // mass flux
    double D = grain->GetHydraulicDiameter(); // = hydraulic diameter, 4* area / perimeter
    double C_s = prop->GetPropellantSpecificHeat(); // specific heat of propellant (NOT combustion gas)
    double rho = prop->GetDensity(); //propellant density
    double C_p = prop->GetSpecificHeatConstantPressure(); //specific heat of combustion products (gas at constant pressure)
    double T_0 = prop->GetAdiabaticFlameTemp();// adiabatic flame temperature
    double mu = prop->GetGasViscosity(); //viscoisty of combustion products
    double Pr = prop->GetPrandtlNumber(); //prandtl number

    double T_s = OpenBurnUtil::g_kSurfaceTemperature; //Temperature of burning propellant surface
    double T_i = OpenBurnUtil::g_kAmbientTemperature; //Base temperature of propellant (degrees K)
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
std::vector<MotorSimDataPoint*> MotorSim::GetResults()
{
    return m_SimResultData;
}
double MotorSim::GetTotalBurnTime() const
{
    return m_TotalBurnTime;
}
void MotorSim::ClearAllData()
{
    for (auto i : m_SimResultData)
    {
        delete i;
    }
    m_SimResultData.clear();
}