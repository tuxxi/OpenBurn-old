#include "propellant.h"

OpenBurnPropellant::OpenBurnPropellant()
    : OpenBurnPropellant("N/A")
{}
OpenBurnPropellant::OpenBurnPropellant(const QString& name)
    : OpenBurnPropellant(name, 0, 0, 0, 0, 1.25)
{}
//dimensionless, dimensionless, feet/sec, slugs/in^3
OpenBurnPropellant::OpenBurnPropellant(const QString& name, double a, double n, double cstar, double rho, double gamma)
    : m_PropellantName(name), m_burnRateCoef(a), m_burnRateExp(n), m_charVel(cstar), m_density(rho), m_specificHeatRatio(gamma),   
    m_PropellantSpecificHeat(0), m_specificHeatConstantPressure(0), m_specificHeatConstantVolume(0),
    m_adiabaticFlameTemp(0), m_molarMass(0), m_gasViscosity(0), m_prandtlNumber(0)
{

}
OpenBurnPropellant::~OpenBurnPropellant()
{
    
}
//dimensionless, dimensionless, feet/sec, slugs/in^3
void OpenBurnPropellant::SetBasicParams(double a, double n, double cstar, double rho, double gamma)
{
    m_burnRateCoef = a;
    m_burnRateExp = n;
    m_charVel = cstar;
    m_density = rho;
    m_specificHeatRatio = gamma;
}
void OpenBurnPropellant::SetAdvancedParams(double Cv, double Cp, double Cs, double Tnot, double molarmass, double mu, double Pr)
{
    m_PropellantSpecificHeat = Cs;
    m_specificHeatConstantPressure = Cp;    
    m_specificHeatConstantVolume = Cv;
    m_adiabaticFlameTemp = Tnot;
    m_molarMass = molarmass;
    m_gasViscosity = mu;
    m_prandtlNumber = Pr;
}
void OpenBurnPropellant::SetPropellantName(const QString& name) { m_PropellantName = name; }
QString& OpenBurnPropellant::GetPropellantName() const { return const_cast<QString&>(m_PropellantName); }

double OpenBurnPropellant::GetBurnRateCoef() const { return m_burnRateCoef; }
double OpenBurnPropellant::GetBurnRateExp() const { return m_burnRateExp; }
double OpenBurnPropellant::GetCharVelocity() const { return m_charVel; }
double OpenBurnPropellant::GetDensity() const{ return m_density; }
//ADVANCED
double OpenBurnPropellant::GetPropellantSpecificHeat() const { return m_PropellantSpecificHeat; }
double OpenBurnPropellant::GetSpecificHeatRatio() const { return m_specificHeatRatio; }
double OpenBurnPropellant::GetSpecificHeatConstantPressure() const{ return m_specificHeatConstantPressure; }
double OpenBurnPropellant::GetSpecificHeatConstantVolume() const{ return m_specificHeatConstantVolume; }
double OpenBurnPropellant::GetAdiabaticFlameTemp() const{ return m_adiabaticFlameTemp; }
double OpenBurnPropellant::GetMolarMass() const{ return m_molarMass; }
double OpenBurnPropellant::GetGasViscosity() const{ return m_gasViscosity; }
double OpenBurnPropellant::GetPrandtlNumber() const{ return m_prandtlNumber; }

void OpenBurnPropellant::ReadJSON(const QJsonObject& object)
{
    m_PropellantName = object["name"].toString();
    m_burnRateCoef = object["a"].toDouble();
    m_burnRateExp = object["n"].toDouble();
    m_charVel = object["CStar"].toDouble();
    m_density = object["rho"].toDouble();
    m_specificHeatRatio = object["gamma"].toDouble();
    
    m_PropellantSpecificHeat = object["Cs"].toDouble();
    m_specificHeatConstantVolume = object["Cv"].toDouble();
    m_specificHeatConstantPressure = object["Cp"].toDouble();
    m_adiabaticFlameTemp = object["T0"].toDouble();
    m_molarMass = object["m"].toDouble();
    m_gasViscosity = object["mu"].toDouble();
    m_prandtlNumber = object["Pr"].toDouble();

}
void OpenBurnPropellant::WriteJSON(QJsonObject &object)
{
    object["name"] = m_PropellantName;
    object["a"] = m_burnRateCoef;
    object["n"] = m_burnRateExp;
    object["CStar"] = m_charVel;
    object["rho"] = m_density;
    object["gamma"] = m_specificHeatRatio;

    //advanced
    object["Cs"] = m_PropellantSpecificHeat;
    object["Cp"] = m_specificHeatConstantPressure;
    object["Cv"] = m_specificHeatConstantVolume;
    object["T0"] = m_adiabaticFlameTemp;
    object["m"] = m_molarMass;
    object["mu"] = m_gasViscosity;
    object["Pr"] = m_prandtlNumber;
}
bool OpenBurnPropellant::operator==(const OpenBurnPropellant& other)
{
    return other.GetPropellantName() == m_PropellantName &&
        other.GetBurnRateCoef() == m_burnRateCoef &&
        other.GetBurnRateExp() == m_burnRateExp &&
        other.GetCharVelocity() == m_charVel && 
        other.GetDensity() == m_density &&
        other.GetSpecificHeatRatio() == m_specificHeatRatio;
}
