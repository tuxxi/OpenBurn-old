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
    double isp;
    MotorSimDataPoint() 
        : motor(nullptr), pressure(0), massflux(0), burnRate(0), time(0), thrust(0), isp(0)
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

    //per unit area:
    static double CalcMassFlux(OpenBurnMotor* motor, double xVal);
    static double CalcMachNumber(OpenBurnMotor* motor, double xVal, double massFlux);

    //bottom of the last grain
    static double CalcCoreMassFlux(OpenBurnMotor* motor);
    static double CalcCoreMachNumber(OpenBurnMotor* motor, double coreMassFlux);
    //per grain:
    //calculates linear burn rate for a given grain
    static double CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    static double CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, MotorSimSettings* settings, double machNumber);

    //whole chamber:
    static double CalcChamberPressure(OpenBurnMotor* motor);
    static double CalcExitMachNumber(OpenBurnMotor* motor);
    static double CalcExitPressure(OpenBurnMotor* motor, double chamberPressure, double exitMach);
    static double CalcThrust(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    static double CalcIdealThrustCoefficient(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    static double CalcIsp(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);

    void RunSim(MotorSimSettings* settings);

    double GetTotalBurnTime() const;
    double GetAvgThrust() const;
    double GetTotalImpulse() const;

    double GetMaxPressure() const;
    double GetMaxMassFlux() const;
    const std::vector<MotorSimDataPoint*>& GetResults() const;

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
