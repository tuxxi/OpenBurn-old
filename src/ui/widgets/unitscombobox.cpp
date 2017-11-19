#include <QDebug>

#include "unitscombobox.h"

UnitsComboBox::UnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : QComboBox(parent),
      m_buddyBox(buddy)
{
    if (buddy)
    {
        buddy->setMinimum(0.0);
        buddy->setMaximum(double(1e6) - 1);
        buddy->setDecimals(3);
        buddy->setSingleStep(0.25f);
    }
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(OnUnitsUpdated(int)));
}
void UnitsComboBox::SetBuddyDoubleSpinBox(QDoubleSpinBox* box)
{
    if (box != nullptr) m_buddyBox = box;
}
QDoubleSpinBox* UnitsComboBox::GetBuddyDoubleSpinBox() { return m_buddyBox; }

//=============================================================================
//*************LENGTH UNITS********************
//=============================================================================
LengthUnitsComboBox::LengthUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetLengthUnits());
    m_prevUnits = OpenBurnUnits::LengthUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void LengthUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::LengthUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertLength(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void LengthUnitsComboBox::SetUnits(OpenBurnUnits::LengthUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::LengthUnits_T LengthUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::LengthUnits_T LengthUnitsComboBox::GetPrevUnits() { return m_prevUnits; }

//=============================================================================
//*************ANGLE UNITS********************
//=============================================================================
AngleUnitsComboBox::AngleUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetAngleUnits());
    m_prevUnits = OpenBurnUnits::AngleUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void AngleUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::AngleUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertAngle(
                m_prevUnits,
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void AngleUnitsComboBox::SetUnits(OpenBurnUnits::AngleUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::AngleUnits_T AngleUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::AngleUnits_T AngleUnitsComboBox::GetPrevUnits() { return m_prevUnits; }

//=============================================================================
//*************PRESSURE UNITS********************
//=============================================================================
PressureUnitsComboBox::PressureUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetPressureUnits());
    m_prevUnits = OpenBurnUnits::PressureUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void PressureUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::PressureUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertPressure(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void PressureUnitsComboBox::SetUnits(OpenBurnUnits::PressureUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::PressureUnits_T PressureUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::PressureUnits_T PressureUnitsComboBox::GetPrevUnits() { return m_prevUnits; }

//=============================================================================
//*************TEMPERATURE UNITS********************
//=============================================================================
TemperatureUnitsComboBox::TemperatureUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetTemperatureUnits());
    m_prevUnits = OpenBurnUnits::TemperatureUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void TemperatureUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::TemperatureUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertTemperature(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void TemperatureUnitsComboBox::SetUnits(OpenBurnUnits::TemperatureUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::TemperatureUnits_T TemperatureUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::TemperatureUnits_T TemperatureUnitsComboBox::GetPrevUnits() { return m_prevUnits; }

//=============================================================================
//*************FORCE UNITS********************
//=============================================================================
ForceUnitsComboBox::ForceUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetForceUnits());
    m_prevUnits = OpenBurnUnits::ForceUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void ForceUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::ForceUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertForce(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void ForceUnitsComboBox::SetUnits(OpenBurnUnits::ForceUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::ForceUnits_T ForceUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::ForceUnits_T ForceUnitsComboBox::GetPrevUnits() { return m_prevUnits; }

//=============================================================================
//*************MASS UNITS********************
//=============================================================================
MassUnitsComboBox::MassUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::GetMassUnits());
    m_prevUnits = OpenBurnUnits::MassUnits_T(currentIndex());
    m_currentUnits = m_prevUnits;
}
void MassUnitsComboBox::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::MassUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertMass(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
void MassUnitsComboBox::SetUnits(OpenBurnUnits::MassUnits_T units)
{
    setCurrentIndex(int(units));
}
OpenBurnUnits::MassUnits_T MassUnitsComboBox::GetCurrentUnits() { return m_currentUnits; }
OpenBurnUnits::MassUnits_T MassUnitsComboBox::GetPrevUnits() { return m_prevUnits; }
