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
OpenBurnSettings::~OpenBurnSettings() {}