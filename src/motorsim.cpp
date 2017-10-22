#include "motorsim.h"
#include "util.h"
#include <QtDebug>
using OpenBurnUtil::g_kGasConstantR;
void MotorSim::SetGrains(std::vector<OpenBurnGrain*> grains)
{
    m_Grains = grains;
}
void MotorSim::AddGrain(OpenBurnGrain* grain)
{
    m_Grains.push_back(grain);
}
void MotorSim::RemoveGrain(OpenBurnGrain *grain)
{
    for (auto i = m_Grains.begin(); i != m_Grains.end(); ++i)
    {
        if (*i == grain)
        {
            m_Grains.erase(i);
        }
    }
}
void MotorSim::RemoveGrain(int index)
{
    m_Grains.erase(m_Grains.begin() + index);
}
void MotorSim::SetNozzle(OpenBurnNozzle* nozz)
{
    m_Nozzle = nozz;
}

void MotorSim::RunSim(double timestep)
{
    CalcAvgPropellant();
    for (auto i : m_Grains)
    {
        i->SetBurnRate(CalcSteadyStateBurnRate(i));
        // TODO: if (erosive)
        //i->SetErosiveBurnRate();
        i->Burn(timestep);
    }
}
//mdot A.K.A Mass flux at given crossflow mach number and port area
double MotorSim::CalcMassFlux(double machNumber, double portArea)
{
    //mdot = M * A * P_0 * sqrt(k / R * T_0) * (1 + (k - 1) / 2 * M^2) ^ - (k + 1) / (2(k-1))
    //P_0 = gas pressure, T_0 = gas temperature, k = ratio specific heats (cp/cv), M = mach number, R = gas constant
    //see https://spaceflightsystems.grc.nasa.gov/education/rocket/mflchk.html
    //and http://propulsion-skrishnan.com/pdf/Erosive%20Burning%20of%20Solid%20Propellants.pdf

    double temp = m_avgPropellant->GetAdiabaticFlameTemp();
    double k = m_avgPropellant->GetSpecificHeatRatio();
    double A = machNumber * portArea * CalcChamberPressure() * qSqrt( (k / (temp * g_kGasConstantR)) );
    double B = 1.0f + (k - 1) / 2 * machNumber * machNumber;
    double C = (k - 1.0f) / 2;
    return A * qPow(B, -1.0f*C);
}
double MotorSim::CalcKn()
{
    double nozzleRadius = (m_Nozzle->GetNozzleThroat() / 2.0f);
    double surfaceArea = 0;
    double nozzleSurfaceArea =  nozzleRadius * nozzleRadius * M_PI;
    for (auto i : m_Grains)
    {
        surfaceArea += i->GetSurfaceArea();
    }
    return surfaceArea / nozzleSurfaceArea;
}
double MotorSim::CalcChamberPressure()
{
    //p = (Kn * a * rho * C* )^(1/(1-n)
    double exponent = 1.0f / (1.0f - m_avgPropellant->GetBurnRateExp());
    double p1 = CalcKn() * m_avgPropellant->GetBurnRateCoef() *
            m_avgPropellant->GetDensity() * m_avgPropellant->GetCharVelocity();
    return qPow(p1, exponent);
}
double MotorSim::CalcSteadyStateBurnRate(OpenBurnGrain* grain)
{
    // r = a * p^n
    OpenBurnPropellant prop = grain->GetPropellantType();
    return prop.GetBurnRateCoef() * qPow(CalcChamberPressure(), prop.GetBurnRateExp());
}
double MotorSim::CalcErosiveBurnRateFactor(OpenBurnGrain* grain, double machNumber)
{
    OpenBurnPropellant prop = grain->GetPropellantType();
    double k = prop.GetSpecificHeatRatio();
    double P_s = CalcChamberPressure(); //steady-state chamber pressure

    //First we have to calculate saint roberts law again, using static pressure at grain surface obtained by:
    //P = P_0(1+ (k-1)/2 * M^2) ^ -k / (k-1)
    double base = 1.0f + (k - 1.0f)/2.0f * machNumber * machNumber;
    double exponent = -1.0f*k / (k - 1.0f);
    double P = P_s * qPow(base, exponent);

    //This burn rate value is __lower__ than saint robert's law due to stagnation pressure considerations.
    //Normally this value would be used as R_0 but the core mach number is not considered in the basic case,
    //So we calculate it here and subtract the difference from R_e at the end.
    double R_0 = prop.GetBurnRateCoef() * qPow(P, prop.GetBurnRateExp());
    double R_diff = CalcSteadyStateBurnRate(grain) - R_0; //this factor will be removed later on

    //For these calculations we need LOTS of new variables!
    double beta =  53; //(experimental, chosen by Lenoir and Robillard)
    double G = CalcMassFlux(machNumber, grain->GetPortArea()); // mass flux
    double D = grain->GetHydraulicDiameter(); // = hydraulic diameter, 4* area / perimeter
    double C_s = prop.GetPropellantSpecificHeat(); // specific heat of propellant (NOT combustion gas)
    double rho = prop.GetDensity(); //propellant density
    double C_p = prop.GetSpecificHeatConstantPressure(); //specific heat of combustion products (gas at constant pressure)
    double T_0 = prop.GetAdiabaticFlameTemp();// adiabatic flame temperature
    double mu = prop.GetGasViscosity(); //viscoisty of combustion products
    double Pr = prop.GetPrandtlNumber(); //prandtl number

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
OpenBurnPropellant* MotorSim::CalcAvgPropellant()
{
    double weighted_a = 0, weighted_n = 0, weighted_cstar = 0, weighted_cpcv = 0;
    //sum up all the propellant properties
    for (auto i : m_Grains)
    {
        //"weight" each property based on the propellant mass in the motor
        double mass = i->GetVolume() * i->GetPropellantType().GetDensity();
        weighted_a += mass * i->GetPropellantType().GetBurnRateCoef();
        weighted_n += mass * i->GetPropellantType().GetBurnRateExp();
        weighted_cstar += mass * i->GetPropellantType().GetCharVelocity();
        weighted_cpcv += mass * i->GetPropellantType().GetSpecificHeatRatio();
    }
    double a = weighted_a / m_Grains.size();
    double n = weighted_n / m_Grains.size();
    double cstar = weighted_cstar / m_Grains.size();
    double cpcv = weighted_cpcv / m_Grains.size();
    return new OpenBurnPropellant(a, n, cstar, cpcv, "OPENBURNDEBUG::AVGPROP");
}
void MotorSim::SwapGrains(int oldPos, int newPos)
{
    //TODO: fix me
    qDebug() << "Grain idx " << oldPos << "swapping with idx " << newPos << "\n";
    qDebug() << "m_Grains has size: " << m_Grains.size() << "\n";

    std::swap(m_Grains[oldPos], m_Grains[newPos]); //std::swap uses move semantics afaik
}
bool MotorSim::HasNozzle()
{
    return m_Nozzle;
}
bool MotorSim::HasGrains()
{
    return m_Grains.size() != 0;
}