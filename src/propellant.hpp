#pragma once
#include <QString>
#include <QJsonObject>

#include <vector>

#include "util.h"

class OpenBurnPropellant
{
public:
    OpenBurnPropellant();
    OpenBurnPropellant(const QString& name);    
    OpenBurnPropellant(const QString& name, double a, double n, double cstar, double rho, double gamma = 1.25);
    virtual ~OpenBurnPropellant() = default;

    bool operator==(const OpenBurnPropellant& other);
    void SetPropellantName(const QString& name);
    QString& GetPropellantName() const;
    void SetBasicParams(double a, double n, double cstar, double rho, double gamma = 1.25);
    void SetAdvancedParams(double Cv, double GetCp, double Cs, double Tnot, double molarmass, double mu, double Pr);

    //BASIC:
    double GetBurnRateCoef() const;
    double GetBurnRateExp() const;
    double GetCharVelocity() const;
    double GetDensity() const;

    //ADVANCED:
    double GetPropellantSpecificHeat() const;
    double GetCp() const;
    double GetCv() const;
    double GetSpecificHeatRatio() const;
    double GetAdiabaticFlameTemp() const;
    double GetMolarMass() const;
    double GetGasViscosity() const;
    double GetPrandtlNumber() const;

    void ReadJSON(const QJsonObject& object);
    void WriteJSON(QJsonObject &object);
private:
    QString m_PropellantName;
    //BASIC:
    double m_burnRateCoef, m_burnRateExp; //a, n
    double m_charVel; //c*, m/s or f/s
    double m_density; //rho, kg/m^3 or slugs/in^3
    double m_specificHeatRatio; // Cp/Cv, gamma
    
    //ADVANCED:
    double m_PropellantSpecificHeat; //C_s
    double m_specificHeatConstantPressure; //C_p
    double m_specificHeatConstantVolume; //C_v
    double m_adiabaticFlameTemp; //T_0, K
    double m_molarMass; //kg/kg mol
    double m_gasViscosity; //mu, kg/m-sec
    double m_prandtlNumber; //Pr
};
typedef std::vector<OpenBurnPropellant> PropellantList;
