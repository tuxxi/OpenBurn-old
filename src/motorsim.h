#pragma once

#include <vector>

#include "motor.h"
#include "settings.h"

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
class MotorSim : public QObject
{
    Q_OBJECT
public:
    MotorSim();
    MotorSim(OpenBurnMotor* initialDesign);

    ~MotorSim();

    void RunSim(MotorSimSettings* settings);

    //per unit area:
    double CalcMassFlux(OpenBurnMotor* motor, double xVal);
    double CalcMachNumber(OpenBurnMotor* motor, double xVal, double massFlux);

    //bottom of the last grain
    double CalcCoreMassFlux(OpenBurnMotor* motor);
    double CalcCoreMachNumber(OpenBurnMotor* motor, double coreMassFlux);
    //per grain:
    //calculates linear burn rate for a given grain
    double CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    double CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, MotorSimSettings* settings, double machNumber);

    //whole chamber:
    double CalcChamberPressure(OpenBurnMotor* motor);
    double CalcExitMachNumber(OpenBurnMotor* motor);
    double CalcExitPressure(OpenBurnMotor* motor, double chamberPressure, double exitMach);
    double CalcThrust(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    double CalcIdealThrustCoefficient(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    double CalcIsp(OpenBurnMotor* motor, MotorSimSettings* settings);

    double GetTotalBurnTime() const;
    double GetAvgThrust() const;
    double GetTotalImpulse() const;

    double GetMaxPressure() const;
    double GetMaxMassFlux() const;
    std::vector<MotorSimDataPoint*>& GetResults();

    void SetDesignMotor(OpenBurnMotor* motor);
signals:
    void SimulationStarted();
    //false if there was an error
    void SimulationFinished(bool success);
private:
    void ClearAllData();
    std::vector<MotorSimDataPoint*> m_SimResultData;
    OpenBurnMotor* m_InitialDesignMotor;

    double m_TotalBurnTime;
    double m_TotalImpulse;
};
