#pragma once

#include <vector>

#include "motor.h"

class MotorSim
{
public:
    MotorSim();
    MotorSim(OpenBurnMotor* initialDesign);

    ~MotorSim();
    //per unit area:
    double CalcMassFlux(OpenBurnMotor* motor, double machNumber, double portArea);

    //per grain:
    //calculates linear burn rate for a given grain
    double CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    double CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, double machNumber);

    //whole chamber:
    double CalcChamberPressure(OpenBurnMotor* motor);

    void RunSim(double timestep = 0.01f);

private:
    std::vector<OpenBurnMotor*> m_SimResultData;
    OpenBurnMotor* m_InitialDesignMotor;

    double m_TotalBurnTime;
};
