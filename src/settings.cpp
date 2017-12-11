#include <QJsonObject>
#include <string>
#include "settings.h"

OpenBurnSettings::OpenBurnSettings()
    : m_LengthUnits(OpenBurnUnits::LengthUnits_T::inches),
    m_AngleUnits(OpenBurnUnits::AngleUnits_T::degrees),
    m_TemperatureUnits(OpenBurnUnits::TemperatureUnits_T::fahrenheit),
    m_PressureUnits(OpenBurnUnits::PressureUnits_T::psi),
    m_ForceUnits(OpenBurnUnits::ForceUnits_T::newtons), //amateur rocketry conventions in the USA are DUMB
    m_MassUnits(OpenBurnUnits::MassUnits_T::pounds_mass),
    m_VelocityUnits(OpenBurnUnits::VelocityUnits_T::feet_per_second),
    m_MassFluxUnits(OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in),
    m_DensityUnits(OpenBurnUnits::DensityUnits_T::lbs_per_in_cu),
    m_redrawOnChanges(true)
{
    
}
MotorSimSettings::MotorSimSettings()
    : MotorSimSettings(14.7f, 70.0f, 0.85f, 0.98f)
{

}
MotorSimSettings::MotorSimSettings(double ambientPress, double ambientTemperature, double twophase, double skinfriction,
                 double timestep)
    : ambientPressure(ambientPress), ambientTemp(ambientTemperature),
    twoPhaseFlowEfficency(twophase),
    skinFrictionEfficency(skinfriction),
    timeStep(timestep)
{
}
void OpenBurnSettings::ReadJSON(const QJsonObject& object)
{
    QJsonObject unitsObject = object["units"].toObject();
    QJsonObject settingsObject = object["settings"].toObject();

    if (unitsObject.isEmpty() || settingsObject.isEmpty())
        return;
        
    m_LengthUnits = OpenBurnUnits::LengthUnits(unitsObject["length"].toInt());
    m_AngleUnits = OpenBurnUnits::AngleUnits(unitsObject["angle"].toInt());
    m_TemperatureUnits = OpenBurnUnits::TemperatureUnits(unitsObject["temperature"].toInt());
    m_PressureUnits = OpenBurnUnits::PressureUnits(unitsObject["pressure"].toInt());
    m_ForceUnits = OpenBurnUnits::ForceUnits(unitsObject["force"].toInt());
    m_MassUnits = OpenBurnUnits::MassUnits(unitsObject["mass"].toInt());
    m_VelocityUnits = OpenBurnUnits::VelocityUnits(unitsObject["velocity"].toInt());
    m_MassFluxUnits = OpenBurnUnits::MassFluxUnits(unitsObject["massflux"].toInt());
    m_DensityUnits = OpenBurnUnits::DensityUnits(unitsObject["density"].toInt());

    m_redrawOnChanges = settingsObject["redraw"].toBool();
}   
void OpenBurnSettings::WriteJSON(QJsonObject &object)
{
    QJsonObject units, settings;
    units["length"] = int(m_LengthUnits.unit);
    units["angle"] = int(m_AngleUnits.unit);
    units["temperature"] = int(m_TemperatureUnits.unit);
    units["pressure"] = int(m_PressureUnits.unit);
    units["force"] = int(m_ForceUnits.unit);
    units["mass"] = int(m_MassUnits.unit);
    units["velocity"] = int(m_VelocityUnits.unit);
    units["massflux"] = int(m_MassFluxUnits.unit);
    units["density"] = int(m_DensityUnits.unit);

    settings["redraw"] = m_redrawOnChanges;
    object["units"] = units;
    object["settings"] = settings;
}
