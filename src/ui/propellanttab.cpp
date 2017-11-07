#include <QLabel>
#include <QTextStream>
#include <QFormLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>

#include <QDebug>

#include "propellanttab.h"

const static QString DEFAULT_NAME = QString("New Propellant");

PropellantTab::PropellantTab(PropellantList* propellants, QWidget* parent)
    : QWidget(parent), m_Propellants(propellants)
{
    SetupUI();
    LoadDatabase("user/propellants.json");
    if (m_Propellants->size() == 0)
    {
        m_gb_edit->setEnabled(false);        
    }
    else
    {
        PropellantComboBox_Changed(0); //finish setting up UI so that we've got the 1st entry highlighted
    }

    connect(m_SavePropButton, SIGNAL(clicked()), this, SLOT(SaveButton_Clicked()));
    connect(m_DeletePropButton, SIGNAL(clicked()), this, SLOT(DeleteButton_Clicked()));
    connect(m_NewPropButton, SIGNAL(clicked()), this, SLOT(NewButton_Clicked()));
    
    connect(m_cb_propSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(PropellantComboBox_Changed(int)));
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
    m_cb_propSelection->setCurrentIndex(0);    
    //add propellants from file
    topHLayout->addWidget(new QLabel(tr("Select Propellant")));
    topHLayout->addWidget(m_cb_propSelection);
    topHLayout->addWidget(m_DeletePropButton = new QPushButton(tr("Delete")));    
    topHLayout->addWidget(m_NewPropButton = new QPushButton(tr("New")));
    m_DeletePropButton->setEnabled(false);
    //edit pane
    m_gb_edit = new QGroupBox(tr("Edit"));
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
        ingredientsLayout->addWidget(ingredient , i, 0);
        ingredientsLayout->addWidget(mass, i, 1);
        m_ingredients.push_back(IngredientLine(ingredient, mass));
    }
    gb_ingredients->setLayout(ingredientsLayout);
    gb_ingredients->setEnabled(false); //disabled for now

    //propellant properties
    QGroupBox* gb_properties = new QGroupBox(tr("Internal Balistic Parameters"));

    QGroupBox* gb_basic = new QGroupBox(tr("Basic"));
    QFormLayout* basicParamsLayout = new QFormLayout;
    basicParamsLayout->addRow("Burn Rate Coef (a)", m_line_propBRCoef = new QLineEdit);
    basicParamsLayout->addRow("Burn Rate Exp (n)", m_line_propBRExp = new QLineEdit);
    basicParamsLayout->addRow("Density (rho)", m_line_propDensity = new QLineEdit);
    basicParamsLayout->addRow("Characteristic Velocity (C*)", m_line_propCStar = new QLineEdit);
    basicParamsLayout->addRow("Specific Heat Ratio (gamma, Cp/Cv)", m_line_propSpecificHeatRatio = new QLineEdit); 
    gb_basic->setLayout(basicParamsLayout);

    QGroupBox* gb_advanced = new QGroupBox(tr("Advanced"));    
    QFormLayout* advancedParamsLayout = new QFormLayout;
    advancedParamsLayout->addRow("Gas Specific Heat, constant pressure (Cp)", m_line_propCp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Specific Heat, constant volume (Cv)", m_line_propCv = new QLineEdit);
    advancedParamsLayout->addRow("Propellant Specific Heat (Cs)", m_line_propSpecificHeat = new QLineEdit);
    advancedParamsLayout->addRow("Molar Mass (m)", m_line_propMolarMass = new QLineEdit);
    advancedParamsLayout->addRow("Adiabatic Flame Temp (T0)", m_line_propAdiabaticFlameTemp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Viscosity (mu)", m_line_propGasViscosity = new QLineEdit);
    advancedParamsLayout->addRow("Prandtl Number (Pr)", m_line_propPrandtlNumber = new QLineEdit);
    
    gb_advanced->setLayout(advancedParamsLayout);
    gb_advanced->setEnabled(false); //disabled for now

    QVBoxLayout* propertiesLayout = new QVBoxLayout;
    propertiesLayout->addWidget(gb_basic);
    propertiesLayout->addWidget(gb_advanced);
    
    gb_properties->setLayout(propertiesLayout);
    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addLayout(topRowEdit, 0, 0, 1, 2);
    controlsLayout->addWidget(gb_ingredients, 3, 0, 4, 1);
    controlsLayout->addWidget(gb_properties, 3, 1, 4, 1);
    
    m_gb_edit->setLayout(controlsLayout);
    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addLayout(topHLayout, 0, 0, 1, 4);
    masterLayout->addWidget(m_gb_edit, 1, 0, 4, 4);

    setLayout(masterLayout);
}
bool PropellantTab::LoadDatabase(const QString& filename)
{
    m_propellantFileName = filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        m_Propellants->clear();
        QJsonArray propellantArray = loadDoc.object()["propellants"].toArray();
        for (int i = 0; i < propellantArray.size(); ++i)
        {
            QJsonObject propellantObject = propellantArray[i].toObject();
            OpenBurnPropellant prop;
            prop.ReadJSON(propellantObject);
            m_Propellants->push_back(prop);
            m_cb_propSelection->addItem(prop.GetPropellantName());
        }
        file.close();
        return true;
    }
    return false;
}
bool PropellantTab::SaveDatabase()
{
    QFile file(m_propellantFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonObject propellantObject;
        QJsonArray propellantArray;
        for (auto i : *m_Propellants)
        {
            QJsonObject prop;
            i.WriteJSON(prop);
            propellantArray.append(prop);
        }
        propellantObject["propellants"] = propellantArray;
        QJsonDocument saveDoc(propellantObject);
        file.write(saveDoc.toJson());
        return true;
    }
    return false;

}
void PropellantTab::PropellantComboBox_Changed(int idx)
{
    if (idx > -1) //is the group box selection valid?
    {
        m_gb_edit->setEnabled(true); 
        m_DeletePropButton->setEnabled(true);

        OpenBurnPropellant propellant = (*m_Propellants)[idx];
        m_cb_propSelection->setCurrentIndex(idx);
        m_line_propName->setText(propellant.GetPropellantName());
        m_line_propBRCoef->setText(QString::number(propellant.GetBurnRateCoef()));
        m_line_propBRExp->setText(QString::number(propellant.GetBurnRateExp()));
        m_line_propDensity->setText(QString::number(propellant.GetDensity()));
        m_line_propCStar->setText(QString::number(propellant.GetCharVelocity()));
        m_line_propSpecificHeatRatio->setText(QString::number(propellant.GetSpecificHeatRatio()));
    }
}
void PropellantTab::ConnectLineEditSignals()
{
    //all of these signals fire the "update propellant" signal
    connect(m_line_propName, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
    connect(m_line_propBRCoef, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
    connect(m_line_propBRExp, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
    connect(m_line_propDensity, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
    connect(m_line_propCStar, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
    connect(m_line_propSpecificHeatRatio, SIGNAL(editingFinished()), this, SLOT(UpdatePropellant()));
}
void PropellantTab::UpdatePropellant()
{
    int idx = m_cb_propSelection->currentIndex() > 0 ? m_cb_propSelection->currentIndex() : 0;
    OpenBurnPropellant prop = (*m_Propellants)[idx];
    m_cb_propSelection->setItemText(idx, m_line_propName->text());    
    prop.SetPropellantName(m_line_propName->text());
    prop.SetBasicParams(
        m_line_propBRCoef->text().toDouble(),
        m_line_propBRExp->text().toDouble(),
        m_line_propCStar->text().toDouble(),
        m_line_propDensity->text().toDouble(),
        m_line_propSpecificHeatRatio->text().toDouble()
    );
}
void PropellantTab::SaveButton_Clicked()
{
    qDebug() << "Saving database!";
    UpdatePropellant();
    SaveDatabase();
}
void PropellantTab::DeleteButton_Clicked()
{
    int oldIndex = m_cb_propSelection->currentIndex() > 0 ? m_cb_propSelection->currentIndex() : 0;    
    QMessageBox::StandardButton resBtn =
        QMessageBox::question( this, "OpenBurn", tr("Are you sure you want to delete propellant: ")
         + (*m_Propellants)[oldIndex].GetPropellantName(),
        QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    
    if (resBtn == QMessageBox::Yes)
    {
        m_Propellants->erase(m_Propellants->begin() + oldIndex);
        m_cb_propSelection->setCurrentIndex(oldIndex);
        m_cb_propSelection->removeItem(oldIndex);
        if (m_Propellants->empty())
        {
            m_gb_edit->setEnabled(false);
            m_DeletePropButton->setEnabled(false);
        }
        SaveDatabase();    
        }
    else
    {
        return;
    }
}
void PropellantTab::NewButton_Clicked()
{
    OpenBurnPropellant prop(DEFAULT_NAME);
    m_Propellants->push_back(prop);
    m_cb_propSelection->addItem(DEFAULT_NAME);
    m_cb_propSelection->setCurrentIndex(m_Propellants->size() - 1);
    SetDefaultValues();

    m_gb_edit->setEnabled(true);
    m_DeletePropButton->setEnabled(true);
}
void PropellantTab::SetDefaultValues()
{
    m_line_propName->setText(DEFAULT_NAME);  
    m_line_propBRCoef->setText("0");
    m_line_propBRExp->setText("0");
    m_line_propCStar->setText("0");
    m_line_propDensity->setText("0");
    m_line_propSpecificHeatRatio->setText("1.25");    
}