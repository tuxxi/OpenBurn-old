#include "motorsim.h"


void MotorSim::RunSim(double timestep)
{
    for (auto i : m_Grains)
    {
        i->Burn(timestep);
    }
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
    double exp = qPow(CalcChamberPressure(), prop.GetBurnRateExp());
    return prop.GetBurnRateCoef() * exp;
}
OpenBurnPropellant MotorSim::CalcAvgPropellant()
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
    return OpenBurnPropellant(a, n, cstar, cpcv);
}
