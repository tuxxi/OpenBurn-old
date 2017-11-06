#pragma once

#include <vector>

#include "motor.h"

struct MotorSimDataPoint
{
    OpenBurnMotor* motor;
    double pressure;
    double massflux;
    double burnRate;
    double time;
    MotorSimDataPoint() : motor(nullptr), pressure(0), massflux(0), burnRate(0), time(0) 
    {}
    ~MotorSimDataPoint()
    {
        delete motor;
    }
};
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

    double GetTotalBurnTime() const;
    std::vector<MotorSimDataPoint*> GetResults();
private:
    void ClearAllData();
    std::vector<MotorSimDataPoint*> m_SimResultData;
    OpenBurnMotor* m_InitialDesignMotor;

    double m_TotalBurnTime;
};
