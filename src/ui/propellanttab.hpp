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

#include <src/settings.h>
#include <src/ui/widgets/unitscombobox.h>
#include "src/propellant.h"

class PropellantTab : public QWidget
{
    Q_OBJECT
public:
    explicit PropellantTab(PropellantList* propellants, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~PropellantTab() = default;
    void UpdateSettings();
    void AddNewPropellantToDatabase(const OpenBurnPropellant& prop);
    bool SaveDatabase();

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
    //Propellant Database
    bool LoadDatabase(const QString& filename);

    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;

    QString m_DatabaseFileName;

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
