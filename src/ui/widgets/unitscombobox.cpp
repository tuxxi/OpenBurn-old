#include <QDebug>

#include "unitscombobox.h"

UnitsComboBox::UnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : QComboBox(parent), m_buddyBox(buddy)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateUnits(int)));
}
UnitsComboBox::~UnitsComboBox() {}
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
    addItems(OpenBurnUnits::g_kLengthUnits);
    prevUnits = OpenBurnUnits::LengthUnits_T(currentIndex());
    currentUnits = prevUnits;
}
LengthUnitsComboBox::~LengthUnitsComboBox() {}
void LengthUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::LengthUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertLength(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::LengthUnits_T LengthUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::LengthUnits_T LengthUnitsComboBox::GetPrevUnits() { return prevUnits; }

//=============================================================================
//*************ANGLE UNITS********************
//=============================================================================
AngleUnitsComboBox::AngleUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::g_kAngleUnits);
    prevUnits = OpenBurnUnits::AngleUnits_T(currentIndex());
    currentUnits = prevUnits;
}
AngleUnitsComboBox::~AngleUnitsComboBox() {}
void AngleUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::AngleUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertAngle(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::AngleUnits_T AngleUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::AngleUnits_T AngleUnitsComboBox::GetPrevUnits() { return prevUnits; }

//=============================================================================
//*************PRESSURE UNITS********************
//=============================================================================
PressureUnitsComboBox::PressureUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::g_kPressureUnits);
    prevUnits = OpenBurnUnits::PressureUnits_T(currentIndex());
    currentUnits = prevUnits;
}
PressureUnitsComboBox::~PressureUnitsComboBox() {}
void PressureUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::PressureUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertPressure(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::PressureUnits_T PressureUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::PressureUnits_T PressureUnitsComboBox::GetPrevUnits() { return prevUnits; }

//=============================================================================
//*************TEMPERATURE UNITS********************
//=============================================================================
TemperatureUnitsComboBox::TemperatureUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::g_kTemperatureUnits);
    prevUnits = OpenBurnUnits::TemperatureUnits_T(currentIndex());
    currentUnits = prevUnits;
}
TemperatureUnitsComboBox::~TemperatureUnitsComboBox() {}
void TemperatureUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::TemperatureUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertTemperature(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::TemperatureUnits_T TemperatureUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::TemperatureUnits_T TemperatureUnitsComboBox::GetPrevUnits() { return prevUnits; }

//=============================================================================
//*************FORCE UNITS********************
//=============================================================================
ForceUnitsComboBox::ForceUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::g_kForceUnits);
    prevUnits = OpenBurnUnits::ForceUnits_T(currentIndex());
    currentUnits = prevUnits;
}
ForceUnitsComboBox::~ForceUnitsComboBox() {}
void ForceUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::ForceUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertForce(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::ForceUnits_T ForceUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::ForceUnits_T ForceUnitsComboBox::GetPrevUnits() { return prevUnits; }

//=============================================================================
//*************MASS UNITS********************
//=============================================================================
MassUnitsComboBox::MassUnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBox(parent, buddy)
{
    addItems(OpenBurnUnits::g_kMassUnits);
    prevUnits = OpenBurnUnits::MassUnits_T(currentIndex());
    currentUnits = prevUnits;
}
MassUnitsComboBox::~MassUnitsComboBox() {}
void MassUnitsComboBox::UpdateUnits(int newIdx)
{
    prevUnits = currentUnits;
    currentUnits = OpenBurnUnits::MassUnits_T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::ConvertMass(
                prevUnits, 
                currentUnits, 
                m_buddyBox->value()));
    }
    emit UnitsChanged(this);
}
OpenBurnUnits::MassUnits_T MassUnitsComboBox::GetCurrentUnits() { return currentUnits; }
OpenBurnUnits::MassUnits_T MassUnitsComboBox::GetPrevUnits() { return prevUnits; }
