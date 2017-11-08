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
    double thrust;
    MotorSimDataPoint() 
        : motor(nullptr), pressure(0), massflux(0), burnRate(0), time(0), thrust(0)
    {}
    ~MotorSimDataPoint()
    {
        delete motor;
    }
};
struct MotorSimSettings
{
    double ambientPressure;
    double twoPhaseFlowEfficency; //% of combustion that is gaseous. Burnsim assumes 85% by default.
    double skinFrictionEfficency; //99% is typical for this value
    MotorSimSettings(double ambient, double twophase, double skinfriction)
        : ambientPressure(ambient), twoPhaseFlowEfficency(twophase), skinFrictionEfficency(skinfriction)
    {}
};
class MotorSim : public QObject
{
    Q_OBJECT
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

    //
    double CalcThrust(OpenBurnMotor* motor, double chamberPressure);
    double CalcIdealThrustCoefficient(OpenBurnMotor* motor, double chamberPressure);
    void RunSim(double timestep = 0.01f);

    double GetTotalBurnTime() const;
    double GetMaxPressure() const;

    double GetAvgThrust() const;
    double GetTotalImpulse() const;
    std::vector<MotorSimDataPoint*> GetResults();
    
signals:
    void SimulationStarted();
    //false if there was an error
    void SimulationFinished(bool success);
private:
    void ClearAllData();
    MotorSimSettings* m_Settings;
    std::vector<MotorSimDataPoint*> m_SimResultData;
    OpenBurnMotor* m_InitialDesignMotor;

    double m_TotalBurnTime;
    double m_TotalImpulse;
};
