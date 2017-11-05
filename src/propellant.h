#pragma once
#include <QString>
#include "util.h"

class OpenBurnPropellant
{
public:
    OpenBurnPropellant();
    OpenBurnPropellant(double a, double n, double cstar, double rho, QString name);
        
    void SetPropellantName(const QString& name);
    QString GetPropellantName();
    void SetBasicParams(double a, double n, double cstar, double rho);
    void SetAdvancedParams(double Cv, double Cp, double Cs, double Tnot, double molarmass, double mu, double Pr);

    //BASIC:
    double GetBurnRateCoef() const;
    double GetBurnRateExp() const;
    double GetCharVelocity() const;
    double GetDensity() const;

    //ADVANCED:
    double GetPropellantSpecificHeat() const;
    double GetSpecificHeatConstantPressure() const;
    double GetSpecificHeatConstantVolume() const;
    double GetSpecificHeatRatio() const;
    double GetAdiabaticFlameTemp() const;
    double GetMolarMass() const;
    double GetGasViscosity() const;
    double GetPrandtlNumber() const;
private:
    QString m_PropellantName;
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
