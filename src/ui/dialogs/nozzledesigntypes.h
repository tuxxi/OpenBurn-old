#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>

#include "src/nozzle.h"
#include "src/settings.h"
#include "src/units.h"
#include "src/ui/widgets/unitscombobox.h"
//this class represents the default design for a nozzle

class OpenBurnDesignNozzle : public QWidget
{
    Q_OBJECT
public:
    explicit OpenBurnDesignNozzle(
        QWidget* parent = nullptr,
        OpenBurnNozzle* seed = nullptr,
        OpenBurnSettings* settings = nullptr);
    virtual ~OpenBurnDesignNozzle();

    double GetThroatDiameter();
    double GetExitDiameter();
    OpenBurnNozzle* GetNozzle();
signals:
    void SIG_DesignUpdated();
protected:
    virtual void SeedValues();
    void AddNewControls(QWidget* widet, int row, int col);

    QDoubleSpinBox *m_throatDiaSpinBox, *m_exitDiaSpinBox;
    LengthUnitsComboBox *m_throatDiaUnits, *m_exitDiaUnits;
    QComboBox *m_nozzleType;

    OpenBurnNozzle* m_seedNozzle;
    OpenBurnSettings* m_GlobalSettings;
private:
    void SetupUI();
    QGridLayout* layout;

};
class ConicalNozzleDesign : public OpenBurnDesignNozzle
{
    Q_OBJECT
public:
    explicit ConicalNozzleDesign(
        QWidget* parent = nullptr,
        ConicalNozzle* seed = nullptr,
        OpenBurnSettings* settings = nullptr);
    virtual ~ConicalNozzleDesign();

    double GetDivergentHalfAngle();
protected:
    virtual void SeedValues() override;
    QDoubleSpinBox *m_halfAngleSpinBox;
    AngleUnitsComboBox *m_halfAngleUnits;
};
