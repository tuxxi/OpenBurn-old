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

#include "src/propellant.h"

typedef std::pair<QComboBox*, QLineEdit*> IngredientLine;

class PropellantTab : public QWidget
{
    Q_OBJECT
public:
    explicit PropellantTab(PropellantList* propellants, QWidget* parent = nullptr);
    ~PropellantTab() = default;
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
    bool SaveDatabase();
    PropellantList* m_Propellants;
    QString m_DatabaseFileName;

    QGroupBox* m_gbEdit;

    QComboBox* m_cbPropSelection;
    QPushButton* m_btnSaveProp, *m_btnDeleteProp, *m_btnNewProp, *m_btnCalculate;
    QLineEdit* m_lnePropName;

    //ingredients
    std::vector<IngredientLine> m_ingredients;
    //Propellant Gas Properties
    //basic
    QLineEdit* m_lnePropBRCoef, *m_lnePropBRExp, *m_lnePropDensity, *m_lnePropCStar;

    //advanced
    QLineEdit* m_lnePropSpecificHeat, *m_lnePropGasSpecificHeatRatio, *m_lnePropGasCp, *m_lnePropGasCv, *m_lnePropAdiabaticFlameTemp,
        *m_lnePropMolarMass, *m_lnePropGasViscosity, *m_lnePropPrandtlNumber;
};
