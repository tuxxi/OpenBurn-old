#include "propellant.h"

OpenBurnPropellant::OpenBurnPropellant() {}
OpenBurnPropellant::OpenBurnPropellant(double a, double n, double cstar, double rho, QString name)
    : m_PropellantName(name), m_burnRateCoef(a), m_burnRateExp(n), m_charVel(cstar), m_density(rho)
{

}

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
void OpenBurnPropellant::SetPropellantName(const QString& name) { m_PropellantName = name; }
QString OpenBurnPropellant::GetPropellantName() { return m_PropellantName; }

double OpenBurnPropellant::GetBurnRateCoef() { return m_burnRateCoef; }
double OpenBurnPropellant::GetBurnRateExp() { return m_burnRateExp; }
double OpenBurnPropellant::GetCharVelocity() { return m_charVel; }
double OpenBurnPropellant::GetDensity() { return m_density; }
//ADVANCED
double OpenBurnPropellant::GetPropellantSpecificHeat() { return m_PropellantSpecificHeat; }
double OpenBurnPropellant::GetSpecificHeatRatio()
{
    return m_specificHeatConstantPressure / (m_specificHeatConstantVolume - OpenBurnUtil::g_kGasConstantR);
}
double OpenBurnPropellant::GetSpecificHeatConstantPressure() { return m_specificHeatConstantPressure; }
double OpenBurnPropellant::GetSpecificHeatConstantVolume() { return m_specificHeatConstantVolume; }
double OpenBurnPropellant::GetAdiabaticFlameTemp() { return m_adiabaticFlameTemp; }
double OpenBurnPropellant::GetMolarMass() { return m_molarMass; }
double OpenBurnPropellant::GetGasViscosity() { return m_gasViscosity; }
double OpenBurnPropellant::GetPrandtlNumber() { return m_prandtlNumber; }
