#include "motorsim.h"

double MotorSim::CalcKn()
{
    double nozzleRadius = (m_Nozzle->GetNozzleThroat() / 2);
    double surfaceArea = 0;
    double nozzleSurfaceArea =  nozzleRadius * nozzleRadius * M_PI;
    for (OpenBurnGrain* i : m_Grains)
    {
        surfaceArea += i->GetSurfaceArea();
    }
    return surfaceArea / nozzleSurfaceArea;
}
