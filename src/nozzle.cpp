#include "nozzle.h"
#include <QtMath>

double ConicalNozzle::GetNozzleThroatArea()
{
    return 0.25f * M_PI * GetNozzleThroat() * GetNozzleThroat();
}
void ConicalNozzle::SetNozzleThroat(double throat) { m_NozzleThroat = throat; }
void ConicalNozzle::SetNozzleExit(double exit) { m_NozzleExit = exit; }
void ConicalNozzle::SetHalfAngle(double angl) { m_HalfAngle = angl; }
//void ConicalNozzle::SetNozzleThroatLen(double len) {}
