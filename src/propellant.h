#pragma once
#include "util.h"

class OpenBurnPropellant
{
public:
    OpenBurnPropellant() {}
    OpenBurnPropellant(double a, double n, double cstar, double rho)
        : m_burnRateCoef(a), m_burnRateExp(n), m_charVel(cstar), m_density(rho)
    {}
    void SetBasicParams(double a, double n, double cstar, double rho);
    void SetAdvancedParams(double Cv, double Cp, double Cs, double Tnot, double molarmass, double mu, double Pr);

    //BASIC:
    double GetBurnRateCoef() { return m_burnRateCoef; }
    double GetBurnRateExp() { return m_burnRateExp; }
    double GetCharVelocity() { return m_charVel; }
    double GetDensity() { return m_density; }

    //ADVANCED:
    double GetPropellantSpecificHeat() { return m_PropellantSpecificHeat; }
    double GetSpecificHeatConstantPressure() { return m_specificHeatConstantPressure; }
    double GetSpecificHeatConstantVolume() { return m_specificHeatConstantVolume; }
    double GetSpecificHeatRatio();
    double GetAdiabaticFlameTemp() { return m_adiabaticFlameTemp; }
    double GetMolarMass() { return m_molarMass; }
    double GetGasViscosity() { return m_gasViscosity; }
    double GetPrandtlNumber() { return m_prandtlNumber; }
private:
    //BASIC:
    double m_burnRateCoef, m_burnRateExp; //a, n
    double m_charVel; //c*, m/s
    double m_density; //rho, kg/m^3

    //ADVANCED:
    double m_PropellantSpecificHeat; //C_s
    double m_specificHeatConstantPressure; //C_p
    double m_specificHeatConstantVolume; //C_v
    double m_adiabaticFlameTemp; //T_0, K
    double m_molarMass; //kg/kg mol
    double m_gasViscosity; //mu, kg/m-sec
    double m_prandtlNumber; //Pr
};
