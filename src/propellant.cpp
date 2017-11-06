#include "propellant.h"

OpenBurnPropellant::OpenBurnPropellant() {}
//dimensionless, dimensionless, feet/sec, slugs/in^3
OpenBurnPropellant::OpenBurnPropellant(double a, double n, double cstar, double rho, const QString& name)
    : m_PropellantName(name), m_burnRateCoef(a), m_burnRateExp(n), m_charVel(cstar), m_density(rho)
{

}
//dimensionless, dimensionless, feet/sec, slugs/in^3
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

double OpenBurnPropellant::GetBurnRateCoef() const { return m_burnRateCoef; }
double OpenBurnPropellant::GetBurnRateExp() const { return m_burnRateExp; }
double OpenBurnPropellant::GetCharVelocity() const { return m_charVel; }
double OpenBurnPropellant::GetDensity() const{ return m_density; }
//ADVANCED
double OpenBurnPropellant::GetPropellantSpecificHeat() const { return m_PropellantSpecificHeat; }
double OpenBurnPropellant::GetSpecificHeatRatio() const
{
    return m_specificHeatConstantPressure / (m_specificHeatConstantVolume - OpenBurnUtil::g_kGasConstantR);
}
double OpenBurnPropellant::GetSpecificHeatConstantPressure() const{ return m_specificHeatConstantPressure; }
double OpenBurnPropellant::GetSpecificHeatConstantVolume() const{ return m_specificHeatConstantVolume; }
double OpenBurnPropellant::GetAdiabaticFlameTemp() const{ return m_adiabaticFlameTemp; }
double OpenBurnPropellant::GetMolarMass() const{ return m_molarMass; }
double OpenBurnPropellant::GetGasViscosity() const{ return m_gasViscosity; }
double OpenBurnPropellant::GetPrandtlNumber() const{ return m_prandtlNumber; }
