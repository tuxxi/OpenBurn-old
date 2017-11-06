#include <QLabel>
#include <QFormLayout>
#include "propellanttab.h"

PropellantTab::PropellantTab(QWidget* parent)
    : QWidget(parent)
{
    SetupUI();
}
PropellantTab::~PropellantTab()
{

}
void PropellantTab::SetupUI()
{
    static const double numIngredients = 10;
    
    //top row
    QHBoxLayout* topHLayout = new QHBoxLayout;    
    m_cb_propSelection = new QComboBox;
    //add propellants from file
    topHLayout->addWidget(new QLabel(tr("Select Propellant")));
    topHLayout->addWidget(m_cb_propSelection);
    topHLayout->addWidget(m_NewPropButton = new QPushButton(tr("New")));
    topHLayout->addWidget(m_DeletePropButton = new QPushButton(tr("Delete")));
    
    //edit pane
    QGroupBox* gb_edit = new QGroupBox(tr("Edit"));
    QHBoxLayout* topRowEdit = new QHBoxLayout;
    topRowEdit->addWidget(new QLabel(tr("Propellant Name")), 2);
    topRowEdit->addWidget(m_line_propName = new QLineEdit, 4);
    topRowEdit->addWidget(m_SavePropButton = new QPushButton(tr("Save")), 1);
    topRowEdit->addStretch();

    //ingredients
    QGroupBox* gb_ingredients = new QGroupBox(tr("Ingredients"));
    //add ingredient table or combobox or whatever 
    QGridLayout* ingredientsLayout = new QGridLayout;
    for (int i = 0; i < numIngredients; i++)
    {   
        QComboBox* ingredient = new QComboBox;
        QLineEdit* mass = new QLineEdit;
        ingredientsLayout->addWidget(ingredient , i, 0, 1, 5);
        ingredientsLayout->addWidget(mass, i, 1, 1, 1);
        m_ingredients.push_back(IngredientLine(ingredient, mass));
    }
    gb_ingredients->setLayout(ingredientsLayout);

    //propellant properties
    QGroupBox* gb_properties = new QGroupBox(tr("Properties"));
    QFormLayout* basicParamsLayout = new QFormLayout;
    basicParamsLayout->addRow("Burn Rate Coef (a)", m_line_propBRCoef = new QLineEdit);
    basicParamsLayout->addRow("Burn Rate Exp (n)", m_line_propBRExp = new QLineEdit);
    basicParamsLayout->addRow("Density (rho)", m_line_propDensity = new QLineEdit);
    basicParamsLayout->addRow("Characteristic Velocity (C*)", m_line_propCStar = new QLineEdit);
    basicParamsLayout->addRow("Specific Heat Ratio (gamma, Cp/Cv)", m_line_propSpecificHeatRatio = new QLineEdit); 
    m_line_propSpecificHeatRatio->setText("1.25"); //default value of 1.25 since many people do not want to bother with advanced stuff

    QFormLayout* advancedParamsLayout = new QFormLayout;

    advancedParamsLayout->addRow("Gas Specific Heat, constant pressure (Cp)", m_line_propCp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Specific Heat, constant volume (Cv)", m_line_propCv = new QLineEdit);
    advancedParamsLayout->addRow("Propellant Specific Heat (Cs)", m_line_propSpecificHeat = new QLineEdit);
    advancedParamsLayout->addRow("Molar Mass (m)", m_line_propMolarMass = new QLineEdit);
    advancedParamsLayout->addRow("Adiabatic Flame Temp (T0)", m_line_propAdiabaticFlameTemp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Viscosity (mu)", m_line_propGasViscosity = new QLineEdit);
    advancedParamsLayout->addRow("Prandtl Number (Pr)", m_line_propPrandtlNumber = new QLineEdit);
    
    QGridLayout* propertiesLayout = new QGridLayout;
    propertiesLayout->addLayout(basicParamsLayout, 0, 0);
    propertiesLayout->addLayout(advancedParamsLayout, 1, 0);
    
    gb_properties->setLayout(propertiesLayout);
    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addLayout(topRowEdit, 0, 0, 1, 2);
    controlsLayout->addWidget(gb_ingredients, 3, 0, 4, 1);
    controlsLayout->addWidget(gb_properties, 3, 1, 4, 1);
    
    gb_edit->setLayout(controlsLayout);
    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addLayout(topHLayout, 0, 0, 1, 4);
    masterLayout->addWidget(gb_edit, 1, 0, 4, 4);

    setLayout(masterLayout);
}