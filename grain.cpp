#include "grain.h"

void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}
// Bates Grains
double BatesGrain::GetSurfaceArea()
{
    double face_area = M_PI * ( m_grainDia * m_grainDia - m_coreDia * m_coreDia );
    double core_area = 2 * M_PI * m_coreDia * m_grainLen;

    return core_area + (2 - m_numInhibitedFaces) * face_area;
}
double BatesGrain::GetVolume()
{
    double face_area = M_PI * ( m_grainDia * m_grainDia - m_coreDia * m_coreDia );
    return face_area * m_grainLen;
}
bool BatesGrain::Burn(double timestep)
{
    double burnDist = (m_rNot + m_rErosive) * timestep;

    m_coreDia += burnDist;
    m_grainLen -= burnDist * (2 - m_numInhibitedFaces);
    if (m_coreDia < m_grainDia) return true;
    else return false;
}
