#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include "src/units.h"

class UnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~UnitsComboBox() = default;
    void SetBuddyDoubleSpinBox(QDoubleSpinBox* box);
    QDoubleSpinBox* GetBuddyDoubleSpinBox();
signals:
    void UnitsChanged(UnitsComboBox* sender);
protected slots:
    virtual void OnUnitsUpdated(int newIdx) = 0;

protected:
    QDoubleSpinBox* m_buddyBox;
};


class LengthUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit LengthUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~LengthUnitsComboBox() = default;
    void SetUnits(OpenBurnUnits::LengthUnits_T units);
    OpenBurnUnits::LengthUnits_T GetCurrentUnits();
    OpenBurnUnits::LengthUnits_T GetPrevUnits();
    //the units combo box is the "buddy" of this double spin box
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::LengthUnits_T m_prevUnits;
    OpenBurnUnits::LengthUnits_T m_currentUnits;
};

class AngleUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit AngleUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~AngleUnitsComboBox() = default;

    void SetUnits(OpenBurnUnits::AngleUnits_T units);

    OpenBurnUnits::AngleUnits_T GetCurrentUnits();
    OpenBurnUnits::AngleUnits_T GetPrevUnits();
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::AngleUnits_T m_prevUnits;
    OpenBurnUnits::AngleUnits_T m_currentUnits;
};

class PressureUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit PressureUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~PressureUnitsComboBox() = default;

    void SetUnits(OpenBurnUnits::PressureUnits_T units);

    OpenBurnUnits::PressureUnits_T GetCurrentUnits();
    OpenBurnUnits::PressureUnits_T GetPrevUnits();
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::PressureUnits_T m_prevUnits;
    OpenBurnUnits::PressureUnits_T m_currentUnits;
};

class TemperatureUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit TemperatureUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~TemperatureUnitsComboBox() = default;

    void SetUnits(OpenBurnUnits::TemperatureUnits_T units);

    OpenBurnUnits::TemperatureUnits_T GetCurrentUnits();
    OpenBurnUnits::TemperatureUnits_T GetPrevUnits();
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::TemperatureUnits_T m_prevUnits;
    OpenBurnUnits::TemperatureUnits_T m_currentUnits;
};
class ForceUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit ForceUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~ForceUnitsComboBox() = default;

    void SetUnits(OpenBurnUnits::ForceUnits_T units);

    OpenBurnUnits::ForceUnits_T GetCurrentUnits();
    OpenBurnUnits::ForceUnits_T GetPrevUnits();
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::ForceUnits_T m_prevUnits;
    OpenBurnUnits::ForceUnits_T m_currentUnits;
};
class MassUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit MassUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    ~MassUnitsComboBox() = default;

    void SetUnits(OpenBurnUnits::MassUnits_T units);

    OpenBurnUnits::MassUnits_T GetCurrentUnits();
    OpenBurnUnits::MassUnits_T GetPrevUnits();
protected slots:
    virtual void OnUnitsUpdated(int newIdx) override;
private:
    OpenBurnUnits::MassUnits_T m_prevUnits;
    OpenBurnUnits::MassUnits_T m_currentUnits;
};
