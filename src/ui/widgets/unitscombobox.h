#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include "src/units.h"

//template classes cannot use Q_OBJECT for signals and slots so we have to use this workaround base class that
//handles all the signals and slots for us
class UnitsComboBoxBase : public QComboBox
{
    Q_OBJECT
public:
    UnitsComboBoxBase(QWidget* parent = nullptr);

signals:
    void UnitsChanged(UnitsComboBoxBase* sender);
protected slots:
    virtual void OnUnitsUpdated(int newIdx);

};
template<typename T, class K>
class UnitsComboBox : public virtual UnitsComboBoxBase
{
public:
    UnitsComboBox(QWidget* parent = nullptr, QWidget* buddy = nullptr);
    virtual ~UnitsComboBox() = default;
        
    //the units combo box is the "buddy" of this widget
    //whenever the units change, the combo box will update the value in this widget
    //currently supports QDoubleSpinBox and QLineEdit
    void SetBuddyWidget(QWidget* wid);
    QWidget* GetBuddyWidget();

    void SetUnits(OpenBurnUnits::OpenBurnUnit<T, K> units);
    OpenBurnUnits::OpenBurnUnit<T, K> GetCurrentUnits();
    OpenBurnUnits::OpenBurnUnit<T, K> GetPrevUnits();

protected:
    virtual void OnUnitsUpdated(int newIdx) override;
    OpenBurnUnits::OpenBurnUnit<T, K> m_prevUnits;
    OpenBurnUnits::OpenBurnUnit<T, K> m_currentUnits;
    QWidget* m_buddyWidget;
};
typedef UnitsComboBox<OpenBurnUnits::LengthUnits_T, OpenBurnUnits::LengthUnits> LengthUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::AngleUnits_T, OpenBurnUnits::AngleUnits> AngleUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::TemperatureUnits_T, OpenBurnUnits::TemperatureUnits> TemperatureUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::PressureUnits_T, OpenBurnUnits::PressureUnits> PressureUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::ForceUnits_T, OpenBurnUnits::ForceUnits> ForceUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::MassUnits_T, OpenBurnUnits::MassUnits> MassUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::VelocityUnits_T, OpenBurnUnits::VelocityUnits> VelocityUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::MassFluxUnits_T, OpenBurnUnits::MassFluxUnits> MassFluxUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::DensityUnits_T, OpenBurnUnits::DensityUnits> DensityUnitsComboBox;
typedef UnitsComboBox<OpenBurnUnits::BurnRateUnits_T , OpenBurnUnits::BurnRateUnits> BurnRateUnitsComboBox;

//These template definitions have to be in the header file so the linker knows what to look for
template<typename T, class K>
UnitsComboBox<T, K>::UnitsComboBox(QWidget* parent, QWidget* buddy)
    : UnitsComboBoxBase(parent),
      m_buddyWidget(buddy)
{
    m_prevUnits = OpenBurnUnits::OpenBurnUnit<T, K>(currentIndex());
    m_currentUnits = m_prevUnits;
    addItems(K::GetUnits());
    if (QDoubleSpinBox* box = qobject_cast<QDoubleSpinBox*>(buddy))
    {
        box->setMinimum(0.0);
        box->setMaximum(double(1e6) - 1);
        box->setDecimals(3);
        box->setSingleStep(0.25f);
    }
}
template<typename T, class K>
void UnitsComboBox<T, K>::SetBuddyWidget(QWidget *wid)
{
    if (wid != nullptr) m_buddyWidget = wid;
}
template<typename T, class K>
QWidget* UnitsComboBox<T, K>::GetBuddyWidget(){ return m_buddyWidget; }

template<typename T, class K>
void UnitsComboBox<T, K>::SetUnits(OpenBurnUnits::OpenBurnUnit<T, K> units)
{
    setCurrentIndex(int(units));
}
template<typename T, class K>
OpenBurnUnits::OpenBurnUnit<T, K> UnitsComboBox<T, K>::GetCurrentUnits()
{
    return m_currentUnits;
}
template<typename T, class K>
OpenBurnUnits::OpenBurnUnit<T, K> UnitsComboBox<T, K>::GetPrevUnits()
{
    return m_prevUnits;
}
template<typename T, class K>
void UnitsComboBox<T, K>::OnUnitsUpdated(int newIdx)
{
    m_prevUnits = m_currentUnits;
    m_currentUnits = OpenBurnUnits::OpenBurnUnit<T, K>(newIdx);
    if (QDoubleSpinBox* box = qobject_cast<QDoubleSpinBox*>(m_buddyWidget))
    {
        box->setValue(
            m_prevUnits.ConvertTo(
                m_currentUnits.unit,
                box->value()));
    }
    if (QLineEdit* lineedit = qobject_cast<QLineEdit*>(m_buddyWidget))
    {
        lineedit->setText(QString::number(
            m_prevUnits.ConvertTo(
            m_currentUnits.unit,
            lineedit->text().toDouble())));
    }
}
