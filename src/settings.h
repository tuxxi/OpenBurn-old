#pragma once
#include <QObject>

#include "src/units.h"

//Global (application-wide) settings
class OpenBurnSettings : public QObject
{
    Q_OBJECT
public:
    OpenBurnSettings();
    ~OpenBurnSettings() = default;
    
    OpenBurnUnits::LengthUnits_T m_LengthUnits;
    OpenBurnUnits::AngleUnits_T m_AngleUnits;
    OpenBurnUnits::TemperatureUnits_T m_TemperatureUnits;
    OpenBurnUnits::PressureUnits_T m_PressureUnits;
    OpenBurnUnits::ForceUnits_T m_ForceUnits;
    OpenBurnUnits::MassUnits_T m_MassUnits;

    bool m_redrawOnChanges;
signals:
    void SettingsChanged();
};

//MotorSim specific settings
struct MotorSimSettings
{
    double ambientPressure;
    double ambientTemp;

    double twoPhaseFlowEfficency; //% of combustion that is gaseous. Burnsim assumes 85% by default.
    double skinFrictionEfficency; //99% is typical for this value
    double timeStep;

    //default settings
    MotorSimSettings()
        : MotorSimSettings(14.7f, 70, 0.85f, 0.98f)
    {}
    MotorSimSettings(double ambientPress, double ambientTemperature, double twophase, double skinfriction, double timestep = 0.01f)
        : ambientPressure(ambientPress), ambientTemp(ambientTemperature),
        twoPhaseFlowEfficency(twophase),
        skinFrictionEfficency(skinfriction),
        timeStep(timestep)
    {}
    ~MotorSimSettings() = default;
};
