#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QJsonObject>

#include <vector>
#include <utility>

#include "src/settings.hpp"
#include "src/ui/widgets/unitscombobox.hpp"
#include "src/propellant.hpp"

class PropellantTab : public QWidget
{
    Q_OBJECT
public:
    explicit PropellantTab(QWidget* parent = nullptr);
    ~PropellantTab() = default;
    void UpdateSettings();

signals:
    void PropellantsUpdated();
private slots:
    void OnSaveButtonClicked();
    void OnDeleteButtonClicked();
    void OnNewButtonClicked();
    void OnPropellantComboBoxIndexChanged(int);

    void OnPropellantUpdated();
private:
    void SetupUI();
    void ConnectLineEditSignals();
    void SetDefaultValues();

    QComboBox* m_cbPropSelection;
    QPushButton* m_btnSaveProp, *m_btnDeleteProp, *m_btnNewProp, *m_btnCalculate;
    QLineEdit* m_lnePropName;

    //Propellant Gas Properties
    //basic
    QGroupBox* m_gbEdit;
    QLineEdit* m_lnePropBRCoef, *m_lnePropBRExp, *m_lnePropDensity, *m_lnePropCStar;
    DensityUnitsComboBox* m_unitsDensity;
    BurnRateUnitsComboBox* m_unitsBRCoef;
    VelocityUnitsComboBox* m_unitsCStar;

    //advanced
    QLineEdit* m_lnePropSpecificHeat, *m_lnePropGasSpecificHeatRatio, *m_lnePropGasCp, *m_lnePropGasCv, *m_lnePropAdiabaticFlameTemp,
        *m_lnePropMolarMass, *m_lnePropGasViscosity, *m_lnePropPrandtlNumber;
};
