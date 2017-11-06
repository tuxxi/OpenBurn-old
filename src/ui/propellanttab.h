#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

#include <vector>
#include <utility>

typedef std::pair<QComboBox*, QLineEdit*> IngredientLine;
class PropellantTab : public QWidget
{
    Q_OBJECT
public:
    explicit PropellantTab(QWidget* parent = nullptr);
    ~PropellantTab();
private:
    void SetupUI();
    
    QComboBox* m_cb_propSelection;
    QPushButton* m_SavePropButton, *m_DeletePropButton, *m_NewPropButton, *m_CalculateButton;
    QLineEdit* m_line_propName;

    //ingredients
    std::vector<IngredientLine> m_ingredients;
    //Propellant Gas Properties
    //basic
    QLineEdit* m_line_propBRCoef, *m_line_propBRExp, *m_line_propDensity, *m_line_propCStar;

    //advanced
    QLineEdit* m_line_propSpecificHeat, *m_line_propSpecificHeatRatio, *m_line_propCp, *m_line_propCv, *m_line_propAdiabaticFlameTemp,
        *m_line_propMolarMass, *m_line_propGasViscosity, *m_line_propPrandtlNumber;
};