#include "nozzle.h"
#include <QtMath>

OpenBurnNozzle::OpenBurnNozzle(double throat, double exit)
    : m_NozzleThroat(throat), m_NozzleExit(exit)
{

}
double OpenBurnNozzle::GetNozzleExit() const { return m_NozzleExit; }
double OpenBurnNozzle::GetNozzleThroat() const { return m_NozzleThroat; }
double OpenBurnNozzle::GetNozzleThroatArea() const
{
    return 0.25f * M_PI * m_NozzleThroat * m_NozzleThroat;
}
double OpenBurnNozzle::GetNozzleExitArea() const
{
    return 0.25f * M_PI * m_NozzleExit * m_NozzleExit;
}
double OpenBurnNozzle::GetNozzleExpansionRatio() const
{
    return GetNozzleExitArea() / GetNozzleThroatArea();
}
void OpenBurnNozzle::SetNozzleThroat(double throat) { m_NozzleThroat = throat; }
void OpenBurnNozzle::SetNozzleExit(double exit) { m_NozzleExit = exit; }

//Conical Nozzle
ConicalNozzle::ConicalNozzle(double throat, double exit, double halfAngle)
    : OpenBurnNozzle(throat, exit), m_HalfAngle(halfAngle)
{

}
double ConicalNozzle::GetHalfAngle() const { return m_HalfAngle; }
double ConicalNozzle::GetThroatLength() const { return m_ThroatLen; }

void ConicalNozzle::SetHalfAngle(double angl) { m_HalfAngle = angl; }
void ConicalNozzle::SetNozzleThroatLen(double len) { m_ThroatLen = len; }
void ConicalNozzle::ReadJSON(const QJsonObject& object)
{
    m_NozzleThroat = object["throat"].toDouble();
    m_NozzleExit =  object["exit"].toDouble();
    m_HalfAngle = object["divergentangle"].toDouble();
    m_ThroatLen = object["throatlength"].toDouble();
}
void ConicalNozzle::WriteJSON(QJsonObject& object)
{
    object["_type"] = "CONICAL";
    object["throat"] = m_NozzleThroat;
    object["exit"] = m_NozzleExit;
    object["divergentangle"] = m_HalfAngle;
    object["throatlength"] = m_ThroatLen;
}
double ConicalNozzle::GetNozzleDivergenceLossFactor() const
{
    return (1.f + qCos(qDegreesToRadians(m_HalfAngle)) ) / 2.f;
}
