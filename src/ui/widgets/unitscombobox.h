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
    OpenBurnUnits::LengthUnits_T GetCurrentUnits();
    OpenBurnUnits::LengthUnits_T GetPrevUnits();
    //the units combo box is the "buddy" of this double spin box
protected slots:
    virtual void UpdateUnits(int newIdx) override;
private:
    OpenBurnUnits::LengthUnits_T prevUnits;
    OpenBurnUnits::LengthUnits_T currentUnits;
};

/*
class AngleUnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AngleUnitsComboBox(QWidget* parent = nullptr);
    ~AngleUnitsComboBox();

signals:

};
class TemperatureUnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TemperatureUnitsComboBox(QWidget* parent = nullptr);
    ~TemperatureUnitsComboBox();

signals:

};
class ForceUnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ForceUnitsComboBox(QWidget* parent = nullptr);
    ~ForceUnitsComboBox();

signals:

};
class MassUnitsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MassUnitsComboBox(QWidget* parent = nullptr);
    ~MassUnitsComboBox();

signals:

};
*/