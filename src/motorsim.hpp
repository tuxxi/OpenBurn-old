#pragma once

#include <vector>
#include <memory>

#include "motor.hpp"
#include "settings.hpp"

struct MotorSimDataPoint
{
    std::unique_ptr<OpenBurnMotor> motor;
    double pressure;
    double massflux;
    double burnRate;
    double time;
    double thrust;
    double isp;
    MotorSimDataPoint() 
        : motor(nullptr), pressure(0), massflux(0), burnRate(0), time(0), thrust(0), isp(0)
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
    static double CalcMassFlux(OpenBurnMotor* motor, double xVal);
    static double CalcMachNumber(OpenBurnMotor* motor, double xVal);

    //bottom of the last grain
    static double CalcCoreMassFlux(OpenBurnMotor* motor);
    static double CalcCoreMachNumber(OpenBurnMotor* motor);
    //per grain:
    //calculates linear burn rate for a given grain
    static double CalcSteadyStateBurnRate(OpenBurnMotor* motor, OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    static double CalcErosiveBurnRateFactor(OpenBurnMotor* motor, OpenBurnGrain* grain, MotorSimSettings* settings, double machNumber);

    //whole chamber:
    static double CalcTotalTemperature(OpenBurnMotor* motor);
    static double CalcChamberPressure(OpenBurnMotor* motor);
    static double CalcExitMachNumber(OpenBurnMotor* motor);
    static double CalcExitPressure(OpenBurnMotor* motor, double chamberPressure, double exitMach);
    static double CalcThrust(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    static double CalcIdealThrustCoefficient(OpenBurnMotor* motor, MotorSimSettings* settings, double chamberPressure);
    static double CalcIsp(OpenBurnMotor* motor, double thrust);

    void RunSim(MotorSimSettings* settings);

    double GetTotalBurnTime() const;
    double GetAvgThrust() const;
    double GetTotalImpulse() const;
    double GetMaxPressure() const;
    double GetMaxMassFlux() const;

    QString GetMotorDesignation() const;
    QString GetFullMotorDesignation() const;
    std::vector<std::unique_ptr<MotorSimDataPoint>>::iterator GetResultsBegin();
    std::vector<std::unique_ptr<MotorSimDataPoint>>::iterator GetResultsEnd();
    MotorSimDataPoint* GetResult(int index);
    size_t GetNumPoints() const;
    bool GetResultsEmpty() const;

    OpenBurnMotor* GetDesignMotor() const;
    void SetDesignMotor(OpenBurnMotor* motor);
signals:
    void SimulationStarted();
    //false if there was an error
    void SimulationFinished(bool success);
private:
    void ClearAllData();
    std::vector<std::unique_ptr<MotorSimDataPoint>> m_SimResultData;
    OpenBurnMotor* m_InitialDesignMotor;

    double m_TotalBurnTime;
    double m_TotalImpulse;
};
