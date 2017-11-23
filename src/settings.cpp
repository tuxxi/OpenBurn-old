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
