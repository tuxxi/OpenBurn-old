#pragma once


class OpenBurnNozzle
{
public:
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
public:
    ConicalNozzle(double throat, double exit, double halfAngle = 15.0f)
        : OpenBurnNozzle(throat, exit), m_HalfAngle(halfAngle)
    {}

    ~ConicalNozzle();
    //TODO: add calculations to account for departures from ideal performance
    //http://rasaero.com/dloads/Departures%20from%20Ideal%20Performance.pdf
private:
    double m_HalfAngle;
    double m_throatLength; //straight cut throat
};
