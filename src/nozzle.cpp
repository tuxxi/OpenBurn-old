#include "nozzle.h"
#include <QtMath>

double ConicalNozzle::GetNozzleThroatArea()
{
    return 0.25f * M_PI * GetNozzleThroat() * GetNozzleThroat();
}
