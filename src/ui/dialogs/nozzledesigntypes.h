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

    virtual ~OpenBurnDesignNozzle() = default;

    double GetThroatDiameter();
    double GetExitDiameter();
    OpenBurnNozzle* GetNozzle();
signals:
    void DesignUpdated();
protected:
    virtual void SeedValues();
    void AddNewControls(QWidget* widet, int row, int col);

    QDoubleSpinBox *m_sbThroatDia, *m_sbExitDia;
    LengthUnitsComboBox *m_unitsThroatDia, *m_unitsExitDia;
    QComboBox *m_cbNozzleType;

    OpenBurnNozzle* m_nozzleSeed;
    OpenBurnSettings* m_GlobalSettings;
private:
    void SetupUI();
    QGridLayout* m_Layout;

};
class ConicalNozzleDesign : public OpenBurnDesignNozzle
{
    Q_OBJECT
public:
    explicit ConicalNozzleDesign(
        QWidget* parent = nullptr,
        ConicalNozzle* seed = nullptr,
        OpenBurnSettings* settings = nullptr);
    ~ConicalNozzleDesign() = default;

    double GetDivergentHalfAngle();
protected:
    virtual void SeedValues() override;
    QDoubleSpinBox *m_sbHalfAngle;
    AngleUnitsComboBox *m_unitsHalfAngle;
};
