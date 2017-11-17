#pragma once
#include <QObject>

#include "src/units.h"

//Global (application-wide) settings
class OpenBurnSettings : public QObject
{
    Q_OBJECT
public:
    OpenBurnSettings();
    ~OpenBurnSettings();
    
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