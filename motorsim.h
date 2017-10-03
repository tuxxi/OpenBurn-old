#pragma once

#include <list>

#include "grain.h"
#include "nozzle.h"

class MotorSim
{
public:
    MotorSim(std::list<OpenBurnGrain*>grains, OpenBurnNozzle* nozzle)
        : m_Grains(grains), m_Nozzle(nozzle)
    {}
    void RunSim();
    double CalcSteadyStateBurnRate(); //based on saint robert's law
    double CalcKn();
private:
    std::list<OpenBurnGrain*> m_Grains; //all grains in the motor
    OpenBurnNozzle *m_Nozzle;
    double m_kn; // ratio of all grain burning faces to nozzle throat area
};
