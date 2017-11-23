#include <QJsonArray>
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
<<<<<<< HEAD
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

=======
void OpenBurnSettings::ReadJSON(const QJsonObject& object)
{
    QJsonObject unitsObject = object["units"].toObject();
    QJsonObject settingsObject = object["settings"].toObject();

    if (unitsObject.isEmpty() || settingsObject.isEmpty())
        return;
        
    m_LengthUnits = OpenBurnUnits::LengthUnits_T(unitsObject["lengthunits"].toInt());
    m_AngleUnits = OpenBurnUnits::AngleUnits_T(unitsObject["angleunits"].toInt());
    m_TemperatureUnits = OpenBurnUnits::TemperatureUnits_T(unitsObject["temperatureunits"].toInt());
    m_PressureUnits = OpenBurnUnits::PressureUnits_T(unitsObject["pressureunits"].toInt());
    m_ForceUnits = OpenBurnUnits::ForceUnits_T(unitsObject["forceunits"].toInt()); 
    m_MassUnits = OpenBurnUnits::MassUnits_T(unitsObject["massunits"].toInt()); 

    m_redrawOnChanges = settingsObject["redrawOnChanges"].toBool();
}   
void OpenBurnSettings::WriteJSON(QJsonObject &object)
{
    QJsonObject units, settings;
    units["lengthunits"] = int(m_LengthUnits);
    units["angleunits"] = int(m_AngleUnits);
    units["temperatureunits"] = int(m_TemperatureUnits);
    units["pressureunits"] = int(m_PressureUnits);
    units["forceunits"] = int(m_ForceUnits);
    units["massunits"] = int(m_MassUnits);

    settings["redrawOnChanges"] = m_redrawOnChanges;
    object["units"] = units;
    object["settings"] = settings;
>>>>>>> 3fca494addf42139cb6ffec2d051c499215990ef
}
