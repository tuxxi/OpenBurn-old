#include "grain.h"

double BatesGrain::GetSurfaceArea()
{
    double face_area = M_PI * ( m_grainDia * m_grainDia - m_coreDia * m_coreDia );
    double core_area = 2 * M_PI * m_coreDia * m_grainLen;

    return core_area + (2 - m_numInhibitedFaces) * face_area;
}
