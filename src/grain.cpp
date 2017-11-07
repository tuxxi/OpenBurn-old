#include <QDebug>

#include "grain.h"
OpenBurnGrain::OpenBurnGrain(double diameter, double length, OpenBurnPropellant prop, int inhibited)
    : m_grainDia(diameter), m_grainLen(length),m_propellantType(prop), m_numInhibitedFaces(inhibited)
{

}
OpenBurnGrain::~OpenBurnGrain()
{    
}
double OpenBurnGrain::GetLength() { return m_grainLen; }
double OpenBurnGrain::GetDiameter() { return m_grainDia; }
int OpenBurnGrain::GetInhibitedFaces() { return m_numInhibitedFaces; }
OpenBurnPropellant& OpenBurnGrain::GetPropellantType() { return m_propellantType; }
void OpenBurnGrain::SetLength(double length) { m_grainLen = length; }
void OpenBurnGrain::SetDiameter(double dia) { m_grainDia = dia; }
void OpenBurnGrain::SetInhibitedFaces(int faces) { m_numInhibitedFaces = faces; }
void OpenBurnGrain::SetPropellantType(OpenBurnPropellant prop) { m_propellantType = prop; }
void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}

BatesGrain::BatesGrain()
    : OpenBurnGrain(0, 0, OpenBurnPropellant()), m_coreDia(0)
{}
// Bates Grains
BatesGrain::BatesGrain(double dia, double coredia, double len, OpenBurnPropellant prop, int inhibitedfaces)
    : OpenBurnGrain(dia, len, prop, inhibitedfaces), m_coreDia(coredia)
{

}
BatesGrain::~BatesGrain()
{
}

double BatesGrain::GetCoreDiameter() { return m_coreDia; }
void BatesGrain::SetCoreDiameter(double dia) { m_coreDia = dia; }
double BatesGrain::GetBurningSurfaceArea()
{
    double face_area = 0.25f * M_PI * ((m_grainDia * m_grainDia) - (m_coreDia * m_coreDia));
    double core_area = M_PI * m_coreDia * m_grainLen;

    return core_area + ((2 - m_numInhibitedFaces) * face_area);
}
double BatesGrain::GetPortArea()
{
    double coreRadius = m_coreDia / 2.0f;
    return M_PI * coreRadius * coreRadius;
}
double BatesGrain::GetVolume()
{
    double radiusSq = (m_grainDia / 2.f) * (m_grainDia / 2.f);
    double coreRadiusSq = (m_coreDia / 2.f) * (m_coreDia / 2.f);
    return M_PI * m_grainLen * (radiusSq - coreRadiusSq);
}
double BatesGrain::GetHydraulicDiameter()
{
    return 4 *( GetPortArea() / (M_PI * m_grainDia) );
}
bool BatesGrain::Burn(double timestep)
{
    if (m_coreDia <= m_grainDia)
    {
        double burnDist = (m_rNot + m_rErosive) * timestep;
        
        m_coreDia += (2 * burnDist);
        m_grainLen -= burnDist * (2 - m_numInhibitedFaces);  
        return true;      
    }
    else
    {
        return false;        
    }
}
bool BatesGrain::IsBurnedOut()
{
    return m_coreDia <= m_grainDia;
}
BatesGrain* BatesGrain::Clone()
{
    return new BatesGrain(*this);
}
void BatesGrain::ReadJSON(const QJsonObject& object, QString& propellantNameReturn)
{
    m_coreDia = object["corediameter"].toDouble();
    m_grainDia = object["diameter"].toDouble();
    m_grainLen = object["length"].toDouble();
    m_numInhibitedFaces = object["inhibited"].toInt();
    propellantNameReturn = object["propellant"].toString();
}
void BatesGrain::WriteJSON(QJsonObject& object)
{
    object["_type"] = "BATES";
    object["length"] = m_grainLen;
    object["diameter"] = m_grainDia;
    object["corediameter"] = m_coreDia;
    object["propellant"] = m_propellantType.GetPropellantName();
    object["inhibited"] = m_numInhibitedFaces;
}