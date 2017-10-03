#pragma once


class OpenBurnNozzle
{
public:
    OpenBurnNozzle() {}
    OpenBurnNozzle(double throat, double exit)
        : m_NozzleThroat(throat), m_NozzleExit(exit)
    {}
    ~OpenBurnNozzle() {}
    virtual double GetNozzleThroat() { return m_NozzleThroat; }
    virtual double GetNozzleExit() { return m_NozzleExit; }

private:
    double m_NozzleThroat; //nozzle throat diameter
    double m_NozzleExit; //nozzle exit diameter
};

class ConicalNozzle : public OpenBurnNozzle
{
    //TODO: add calculation to fix
private:
    double m_HalfAngle;
};
