#include <QDebug>

#include "grain.h"
OpenBurnGrain::OpenBurnGrain(double diameter, double length, OpenBurnPropellant prop, int inhibited)
    : m_GrainDia(diameter), m_GrainLen(length),m_Propellant(prop), m_NumInhibited(inhibited), m_isBurnedOut(false)
{

}
double OpenBurnGrain::GetLength() { return m_GrainLen; }
double OpenBurnGrain::GetDiameter() { return m_GrainDia; }
int OpenBurnGrain::GetInhibitedFaces() { return m_NumInhibited; }
OpenBurnPropellant& OpenBurnGrain::GetPropellantType() { return m_Propellant; }
bool OpenBurnGrain::GetIsBurnedOut() { return m_isBurnedOut; }
void OpenBurnGrain::SetLength(double length) { m_GrainLen = length; }
void OpenBurnGrain::SetDiameter(double dia) { m_GrainDia = dia; }
void OpenBurnGrain::SetInhibitedFaces(int faces) { m_NumInhibited = faces; }
void OpenBurnGrain::SetPropellantType(OpenBurnPropellant prop) { m_Propellant = prop; }
void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}

CylindricalGrain::CylindricalGrain()
    : OpenBurnGrain(0, 0, OpenBurnPropellant()), m_CoreDia(0)
{}
// Bates Grains
CylindricalGrain::CylindricalGrain(double dia, double coredia, double len, OpenBurnPropellant prop, int inhibited)
    : OpenBurnGrain(dia, len, prop, inhibited), m_CoreDia(coredia)
{

}
double CylindricalGrain::GetCoreDiameter() { return m_CoreDia; }
void CylindricalGrain::SetCoreDiameter(double dia) { m_CoreDia = dia; }
double CylindricalGrain::GetBurningSurfaceArea()
{
    double face_area = 0.25f * M_PI * ((m_GrainDia * m_GrainDia) - (m_CoreDia * m_CoreDia));
    double core_area = M_PI * m_CoreDia * m_GrainLen;

    return core_area + ((2 - m_NumInhibited) * face_area);
}
double CylindricalGrain::GetPortArea()
{
    double coreRadius = m_CoreDia / 2.0f;
    return M_PI * coreRadius * coreRadius;
}
double CylindricalGrain::GetVolume()
{
    double radiusSq = (m_GrainDia / 2.f) * (m_GrainDia / 2.f);
    double coreRadiusSq = (m_CoreDia / 2.f) * (m_CoreDia / 2.f);
    return M_PI * m_GrainLen * (radiusSq - coreRadiusSq);
}
double CylindricalGrain::GetHydraulicDiameter()
{
    return 4 *( GetPortArea() / (M_PI * m_GrainDia) );
}
bool CylindricalGrain::Burn(double timestep)
{
    if (m_CoreDia <= m_GrainDia)
    {
        double burnDist = (m_rNot + m_rErosive) * timestep;
        
        m_CoreDia += (2 * burnDist);
        m_GrainLen -= burnDist * (2 - m_NumInhibited);
        return true;      
    }
    else
    {
        m_isBurnedOut = true;        
        return false;        
    }
}
bool CylindricalGrain::IsBurnedOut()
{
    return m_CoreDia <= m_GrainDia;
}
CylindricalGrain* CylindricalGrain::Clone()
{
    return new CylindricalGrain(*this);
}
void CylindricalGrain::ReadJSON(const QJsonObject& object, QString& propellantNameReturn)
{
    m_CoreDia = object["corediameter"].toDouble();
    m_GrainDia = object["diameter"].toDouble();
    m_GrainLen = object["length"].toDouble();
    m_NumInhibited = object["inhibited"].toInt();
    propellantNameReturn = object["propellant"].toString();
}
void CylindricalGrain::WriteJSON(QJsonObject& object)
{
    object["_type"] = "BATES";
    object["length"] = m_GrainLen;
    object["diameter"] = m_GrainDia;
    object["corediameter"] = m_CoreDia;
    object["propellant"] = m_Propellant.GetPropellantName();
    object["inhibited"] = m_NumInhibited;
}
