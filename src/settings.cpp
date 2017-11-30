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
        
    m_LengthUnits = OpenBurnUnits::LengthUnits(unitsObject["lengthunits"].toInt());
    m_AngleUnits = OpenBurnUnits::AngleUnits(unitsObject["angleunits"].toInt());
    m_TemperatureUnits = OpenBurnUnits::TemperatureUnits(unitsObject["temperatureunits"].toInt());
    m_PressureUnits = OpenBurnUnits::PressureUnits(unitsObject["pressureunits"].toInt());
    m_ForceUnits = OpenBurnUnits::ForceUnits(unitsObject["forceunits"].toInt());
    m_MassUnits = OpenBurnUnits::MassUnits(unitsObject["massunits"].toInt());

    m_redrawOnChanges = settingsObject["redrawOnChanges"].toBool();
}   
void OpenBurnSettings::WriteJSON(QJsonObject &object)
{
    QJsonObject units, settings;
    units["lengthunits"] = int(m_LengthUnits.unit);
    units["angleunits"] = int(m_AngleUnits.unit);
    units["temperatureunits"] = int(m_TemperatureUnits.unit);
    units["pressureunits"] = int(m_PressureUnits.unit);
    units["forceunits"] = int(m_ForceUnits.unit);
    units["massunits"] = int(m_MassUnits.unit);

    settings["redrawOnChanges"] = m_redrawOnChanges;
    object["units"] = units;
    object["settings"] = settings;
}
