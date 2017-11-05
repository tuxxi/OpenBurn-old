#pragma once

#include <vector>

#include "motor.h"

class MotorSim
{
public:
    MotorSim();
    MotorSim(OpenBurnMotor* motor);
    //per unit area:
    double CalcMassFlux(double machNumber, double portArea);

    //per grain:
    //calculates linear burn rate for a given grain
    double CalcSteadyStateBurnRate(OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    double CalcErosiveBurnRateFactor(OpenBurnGrain* grain, double machNumber);

    //whole chamber:
    double CalcChamberPressure();
    double CalcKn(); //instantaneous kn calc, for grains in motor that have regressed a given ammount

    void RunSim(double timestep = 0.01f);

private:
    OpenBurnMotor* m_Motor;
};
