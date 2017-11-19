#pragma once
#include <QJsonObject>

class OpenBurnNozzle
{
public:
    OpenBurnNozzle(double throat, double exit);
    virtual ~OpenBurnNozzle() = default;
    virtual void SetNozzleThroat(double);
    virtual void SetNozzleExit(double);

    virtual double GetNozzleThroatArea() const;
    virtual double GetNozzleExitArea() const;
    virtual double GetNozzleThroat() const;
    virtual double GetNozzleExit() const;
    virtual double GetNozzleExpansionRatio() const;

    virtual double GetNozzleDivergenceLossFactor() const = 0;
    virtual void ReadJSON(const QJsonObject& object) = 0;
    virtual void WriteJSON(QJsonObject &object) = 0;

protected:
    double m_NozzleThroat; //nozzle throat diameter
    double m_NozzleExit; //nozzle exit diameter
};

class ConicalNozzle : public OpenBurnNozzle
{
public:
    ConicalNozzle(double throat, double exit, double halfAngle = 15.0f);
    virtual ~ConicalNozzle() = default;

    void SetHalfAngle(double);
    void SetNozzleThroatLen(double);
    double GetHalfAngle() const;
    double GetThroatLength() const;
    
    virtual double GetNozzleDivergenceLossFactor() const override;
    virtual void ReadJSON(const QJsonObject& object) override;
    virtual void WriteJSON(QJsonObject &object) override;

    //http://rasaero.com/dloads/Departures%20from%20Ideal%20Performance.pdf
private:
    double m_HalfAngle;
    double m_ThroatLen; //straight cut throat TODO: delete/move to new class maybe?
};
