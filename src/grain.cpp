#include <QDebug>

#include "grain.h"
OpenBurnGrain::OpenBurnGrain(double diameter, double length, OpenBurnPropellant* prop, int inhibited)
    : m_grainDia(diameter), m_grainLen(length),m_prop(prop), m_numInhibitedFaces(inhibited)
{

}
OpenBurnGrain::~OpenBurnGrain()
{    
}
double OpenBurnGrain::GetLength() { return m_grainLen; }
double OpenBurnGrain::GetDiameter() { return m_grainDia; }
int OpenBurnGrain::GetInhibitedFaces() { return m_numInhibitedFaces; }
OpenBurnPropellant* OpenBurnGrain::GetPropellantType() { return m_prop; }
void OpenBurnGrain::SetLength(double length) { m_grainLen = length; }
void OpenBurnGrain::SetDiameter(double dia) { m_grainDia = dia; }
void OpenBurnGrain::SetInhibitedFaces(int faces) { m_numInhibitedFaces = faces; }
void OpenBurnGrain::SetPropellantType(OpenBurnPropellant* prop) { m_prop = prop; }
void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}

// Bates Grains
BatesGrain::BatesGrain(double dia, double coredia, double len, OpenBurnPropellant* prop, int inhibitedfaces)
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
    double face_area = 0.25f * M_PI * ((m_grainDia * m_grainDia) - (m_coreDia * m_coreDia));
    return face_area * m_grainLen;
}
double BatesGrain::GetHydraulicDiameter()
{
    return 4 *( GetPortArea() / (M_PI * m_grainDia) );
}
bool BatesGrain::Burn(double timestep)
{
    double burnDist = (m_rNot + m_rErosive) * timestep;

    m_coreDia += burnDist;
    m_grainLen -= burnDist * (2 - m_numInhibitedFaces);
    if (m_coreDia < m_grainDia) return true;
    else return false;
}
