#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include "src/units.h"

//template classes cannot use Q_OBJECT for signals and slots so we have to use this workaround base class that
//handles all the signals and slots for us
class UnitsComboBoxBase : public QComboBox
{
    Q_OBJECT;
public:
    UnitsComboBoxBase(QWidget* parent = nullptr);

signals:
    void UnitsChanged(UnitsComboBoxBase* sender);
protected slots:
    virtual void OnUnitsUpdated(int newIdx);

};
template<class T>
class UnitsComboBox : public virtual UnitsComboBoxBase
{
public:
    UnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~UnitsComboBox() = default;
        
    //the units combo box is the "buddy" of this double spin box
    void SetBuddyDoubleSpinBox(QDoubleSpinBox* box);
    QDoubleSpinBox* GetBuddyDoubleSpinBox();

    void SetUnits(T units);
    T GetCurrentUnits();
    T GetPrevUnits();

protected:
    virtual void OnUnitsUpdated(int newIdx) override;
    T m_prevUnits;
    T m_currentUnits;
    QDoubleSpinBox* m_buddyBox;
};
typedef UnitsComboBox<OpenBurnUnits::LengthUnits_T> LengthUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::AngleUnits_T> AngleUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::TemperatureUnits_T> TemperatureUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::PressureUnits_T> PressureUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::ForceUnits_T> ForceUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::MassUnits_T> MassUnitsComboBox;

//These template definitions have to be in the header file so the linker knows what to look for
template<class T>
UnitsComboBox<T>::UnitsComboBox(QWidget* parent, QDoubleSpinBox* buddy)
    : UnitsComboBoxBase(parent),
      m_buddyBox(buddy)
{
    m_prevUnits = T(currentIndex());
    m_currentUnits = m_prevUnits;
    addItems(OpenBurnUnits::GetUnits<T>());
    if (m_buddyBox)
    {
        m_buddyBox->setMinimum(0.0);
        m_buddyBox->setMaximum(double(1e6) - 1);
        m_buddyBox->setDecimals(3);
        m_buddyBox->setSingleStep(0.25f);
    }
}
template<class T>
void UnitsComboBox<T>::SetBuddyDoubleSpinBox(QDoubleSpinBox* box)
{
    if (box != nullptr) m_buddyBox = box;
}
template<class T> 
QDoubleSpinBox* UnitsComboBox<T>::GetBuddyDoubleSpinBox() { return m_buddyBox; }

template<class T>
void UnitsComboBox<T>::SetUnits(T units)
{
    setCurrentIndex(int(units));
}
template<class T>
T UnitsComboBox<T>::GetCurrentUnits()
{
    return m_currentUnits;
}
template<class T>
T UnitsComboBox<T>::GetPrevUnits()
{
    return m_prevUnits;
}
template<class T>
void UnitsComboBox<T>::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = T(newIdx);
    if (m_buddyBox)
    {
        m_buddyBox->setValue(
            OpenBurnUnits::Convert<T>(
                m_prevUnits, 
                m_currentUnits, 
                m_buddyBox->value()));
    }
}