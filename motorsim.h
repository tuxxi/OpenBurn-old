#pragma once

#include <list>

#include "grain.h"
#include "nozzle.h"

class MotorSim
{
public:
    MotorSim(std::list<OpenBurnGrain*>grains, OpenBurnNozzle* nozzle)
        : m_Grains(grains), m_Nozzle(nozzle)
    {
        m_avgPropellant = &CalcAvgPropellant();
    }
    //calculates linear burn rate for a given grain
    double CalcSteadyStateBurnRate(OpenBurnGrain*); //based on saint robert's law. No erosive burning
    double CalcErosiveBurnRateFactor(OpenBurnGrain*); //multiplicative law

    //whole chamber:
    double CalcChamberPressure();
    double CalcKn();

    void RunSim(double timestep = 0.01f);

private:
    std::list<OpenBurnGrain*> m_Grains; //all grains in the motor
    OpenBurnNozzle *m_Nozzle;
    //WARNING - APPROXIMATION TIME --- average of all grains in motor because idk what im doing

    //weighted average based on mass of propellant
    OpenBurnPropellant CalcAvgPropellant();
    OpenBurnPropellant* m_avgPropellant;
};
