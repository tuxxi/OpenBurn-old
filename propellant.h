#pragma once

class OpenBurnPropellant
{
public:
    OpenBurnPropellant() {}
    OpenBurnPropellant(double a, double n, double cstar, double rho, double k = 1.25)
        : m_burnRateCoef(a), m_burnRateExp(n), m_charVel(cstar), m_density(rho), m_specificHeatRatio(k)
    {}
    double GetBurnRateCoef() { return m_burnRateCoef; }
    double GetBurnRateExp() { return m_burnRateExp; }
    double GetCharVelocity() { return m_charVel; }
    double GetDensity() { return m_density; }
    double GetSpecificHeatRatio() { return m_specificHeatRatio; }
private:
    double m_burnRateCoef, m_burnRateExp; //a, n
    double m_charVel; //c*
    double m_density; //rho
    double m_specificHeatRatio; //k
};
