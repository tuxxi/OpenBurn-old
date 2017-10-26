#include "grain.h"
BatesGrain::BatesGrain(double dia, double coredia, double len, OpenBurnPropellant prop, int inhibitedfaces)
    : OpenBurnGrain(prop), m_grainDia(dia), m_coreDia(coredia), m_grainLen(len), m_numInhibitedFaces(inhibitedfaces)
{

}
BatesGrain::~BatesGrain()
{

}
void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}

// Bates Grains
double BatesGrain::GetSurfaceArea()
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
