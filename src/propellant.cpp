#include "propellant.h"

void OpenBurnPropellant::SetBasicParams(double a, double n, double cstar, double rho)
{
    m_burnRateCoef = a;
    m_burnRateExp = n;
    m_charVel = cstar;
    m_density = rho;
}
void OpenBurnPropellant::SetAdvancedParams(double Cv, double Cp, double Cs, double Tnot, double molarmass, double mu, double Pr)
{
    m_PropellantSpecificHeat = Cs;
    m_specificHeatConstantVolume = Cv;
    m_specificHeatConstantPressure = Cp;
    m_adiabaticFlameTemp = Tnot;
    m_molarMass = molarmass;
    m_gasViscosity = mu;
    m_prandtlNumber = Pr;
}
double OpenBurnPropellant::GetSpecificHeatRatio()
{
    return m_specificHeatConstantPressure / (m_specificHeatConstantVolume - OpenBurnUtil::g_kGasConstantR);
}

