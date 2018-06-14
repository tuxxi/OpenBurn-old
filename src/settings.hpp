#pragma once
#include <QObject>
#include <QJsonObject>

#include "units.hpp"

//Global (application-wide) settings
class OpenBurnSettings : public QObject
{
    Q_OBJECT
public:
    //TODO: make settings emit changes via accessor and mutator functions

    OpenBurnSettings();
    ~OpenBurnSettings() = default;
    
    void ReadJSON(const QJsonObject& object);
    void WriteJSON(QJsonObject &object);

    OpenBurnUnits::LengthUnits m_LengthUnits;
    OpenBurnUnits::AngleUnits m_AngleUnits;
    OpenBurnUnits::TemperatureUnits m_TemperatureUnits;
    OpenBurnUnits::PressureUnits m_PressureUnits;
    OpenBurnUnits::ForceUnits m_ForceUnits;
    OpenBurnUnits::MassUnits m_MassUnits;
    OpenBurnUnits::VelocityUnits m_VelocityUnits;
    OpenBurnUnits::MassFluxUnits m_MassFluxUnits;
    OpenBurnUnits::DensityUnits m_DensityUnits;
    OpenBurnUnits::BurnRateUnits m_BurnRateUnits;

    bool m_redrawOnChanges;
signals:
    void SettingsChanged();
};

//MotorSim specific settings
class MotorSimSettings : public QObject
{
    Q_OBJECT
public:
    //TODO: make settings emit changes via accessor and mutator functions

    //default settings
    MotorSimSettings();
    MotorSimSettings(double ambientPress, double ambientTemperature, double twophase, double skinfriction,
                     double timestep = 0.01f);
    ~MotorSimSettings() = default;

    double ambientPressure;
    double ambientTemp;

    double twoPhaseFlowEfficency; //% of combustion that is gaseous. Burnsim assumes 85% by default.
    double skinFrictionEfficency; //98-99% is typical for this value
    double timeStep;
signals:
    void SettingsChanged();
};
