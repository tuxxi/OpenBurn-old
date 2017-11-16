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


//LEN
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
