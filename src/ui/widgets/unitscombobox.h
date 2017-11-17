#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include "src/units.h"

class UnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~UnitsComboBox();
    void SetBuddyDoubleSpinBox(QDoubleSpinBox* box);
    QDoubleSpinBox* GetBuddyDoubleSpinBox();
signals:
    void UnitsChanged(UnitsComboBox* sender);
protected slots:
    virtual void UpdateUnits(int newIdx) = 0;

protected:
    QDoubleSpinBox* m_buddyBox;
};


class LengthUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit LengthUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~LengthUnitsComboBox();
    void SetUnits(OpenBurnUnits::LengthUnits_T units);
    OpenBurnUnits::LengthUnits_T GetCurrentUnits();
    OpenBurnUnits::LengthUnits_T GetPrevUnits();
    //the units combo box is the "buddy" of this double spin box
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::LengthUnits_T prevUnits;
    OpenBurnUnits::LengthUnits_T currentUnits;
};

class AngleUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit AngleUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~AngleUnitsComboBox();

    void SetUnits(OpenBurnUnits::AngleUnits_T units);

    OpenBurnUnits::AngleUnits_T GetCurrentUnits();
    OpenBurnUnits::AngleUnits_T GetPrevUnits();
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::AngleUnits_T prevUnits;
    OpenBurnUnits::AngleUnits_T currentUnits;
};

class PressureUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit PressureUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~PressureUnitsComboBox();

    void SetUnits(OpenBurnUnits::PressureUnits_T units);

    OpenBurnUnits::PressureUnits_T GetCurrentUnits();
    OpenBurnUnits::PressureUnits_T GetPrevUnits();
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::PressureUnits_T prevUnits;
    OpenBurnUnits::PressureUnits_T currentUnits;
};

class TemperatureUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit TemperatureUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~TemperatureUnitsComboBox();

    void SetUnits(OpenBurnUnits::TemperatureUnits_T units);

    OpenBurnUnits::TemperatureUnits_T GetCurrentUnits();
    OpenBurnUnits::TemperatureUnits_T GetPrevUnits();
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::TemperatureUnits_T prevUnits;
    OpenBurnUnits::TemperatureUnits_T currentUnits;
};
class ForceUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit ForceUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~ForceUnitsComboBox();

    void SetUnits(OpenBurnUnits::ForceUnits_T units);

    OpenBurnUnits::ForceUnits_T GetCurrentUnits();
    OpenBurnUnits::ForceUnits_T GetPrevUnits();
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::ForceUnits_T prevUnits;
    OpenBurnUnits::ForceUnits_T currentUnits;
};
class MassUnitsComboBox : public UnitsComboBox
{
    Q_OBJECT
public:
    explicit MassUnitsComboBox(QWidget* parent = nullptr, QDoubleSpinBox* buddy = nullptr);
    virtual ~MassUnitsComboBox();

    void SetUnits(OpenBurnUnits::MassUnits_T units);

    OpenBurnUnits::MassUnits_T GetCurrentUnits();
    OpenBurnUnits::MassUnits_T GetPrevUnits();
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::MassUnits_T prevUnits;
    OpenBurnUnits::MassUnits_T currentUnits;
};
