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
    : QWidget(parent),
      m_Propellants(propellants)
{
    SetupUI();
    LoadDatabase("user/propellants.json");
    if (m_Propellants->size() == 0)
    {
        m_gbEdit->setEnabled(false);        
    }
    else
    {
        OnPropellantComboBoxIndexChanged(0); //finish setting up UI so that we've got the 1st entry highlighted
    }

    connect(m_btnSaveProp, &QPushButton::clicked,
            this, &PropellantTab::OnSaveButtonClicked);
    connect(m_btnDeleteProp, &QPushButton::clicked,
            this, &PropellantTab::OnDeleteButtonClicked);
    connect(m_btnNewProp, &QPushButton::clicked,
            this, &PropellantTab::OnNewButtonClicked);
    
    //new function pointer syntax does not work for overloaded signals or slots >.<
    connect(m_cbPropSelection, SIGNAL(currentIndexChanged(int)),
            this, SLOT(OnPropellantComboBoxIndexChanged(int)));
}

void PropellantTab::SetupUI()
{
    static const double numIngredients = 10;
    
    //top row
    QHBoxLayout* topHLayout = new QHBoxLayout;    
    m_cbPropSelection = new QComboBox;
    m_cbPropSelection->setCurrentIndex(0);    
    //add propellants from file
    topHLayout->addWidget(new QLabel(tr("Select Propellant")));
    topHLayout->addWidget(m_cbPropSelection);
    topHLayout->addWidget(m_btnDeleteProp = new QPushButton(tr("Delete")));    
    topHLayout->addWidget(m_btnNewProp = new QPushButton(tr("New")));
    m_btnDeleteProp->setEnabled(false);
    //edit pane
    m_gbEdit = new QGroupBox(tr("Edit"));
    QHBoxLayout* topRowEdit = new QHBoxLayout;
    topRowEdit->addWidget(new QLabel(tr("Propellant Name")), 2);
    topRowEdit->addWidget(m_lnePropName = new QLineEdit, 4);
    topRowEdit->addWidget(m_btnSaveProp = new QPushButton(tr("Save")), 1);
    topRowEdit->addStretch();

    //propellant properties
    QGroupBox* gb_properties = new QGroupBox(tr("Internal Balistic Parameters"));

    QGroupBox* gb_basic = new QGroupBox(tr("Basic"));
    QFormLayout* basicParamsLayout = new QFormLayout;
    basicParamsLayout->addRow("Burn Rate Coef (a)", m_lnePropBRCoef = new QLineEdit);
    basicParamsLayout->addRow("Burn Rate Exp (n)", m_lnePropBRExp = new QLineEdit);
    basicParamsLayout->addRow("Density (rho)", m_lnePropDensity = new QLineEdit);
    basicParamsLayout->addRow("Characteristic Velocity (C*)", m_lnePropCStar = new QLineEdit);
    basicParamsLayout->addRow("Specific Heat Ratio (gamma, Cp/Cv)", m_lnePropGasSpecificHeatRatio = new QLineEdit); 
    gb_basic->setLayout(basicParamsLayout);

    QGroupBox* gb_advanced = new QGroupBox(tr("Advanced"));    
    QFormLayout* advancedParamsLayout = new QFormLayout;
    advancedParamsLayout->addRow("Gas Specific Heat, constant pressure (Cp)", m_lnePropGasCp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Specific Heat, constant volume (Cv)", m_lnePropGasCv = new QLineEdit);
    advancedParamsLayout->addRow("Propellant Specific Heat (Cs)", m_lnePropSpecificHeat = new QLineEdit);
    advancedParamsLayout->addRow("Molar Mass (m)", m_lnePropMolarMass = new QLineEdit);
    advancedParamsLayout->addRow("Adiabatic Flame Temp (T0)", m_lnePropAdiabaticFlameTemp = new QLineEdit);
    advancedParamsLayout->addRow("Gas Viscosity (mu)", m_lnePropGasViscosity = new QLineEdit);
    advancedParamsLayout->addRow("Prandtl Number (Pr)", m_lnePropPrandtlNumber = new QLineEdit);
    
    gb_advanced->setLayout(advancedParamsLayout);
    gb_advanced->setEnabled(false); //disabled for now

    QVBoxLayout* propertiesLayout = new QVBoxLayout;
    propertiesLayout->addWidget(gb_basic);
    propertiesLayout->addWidget(gb_advanced);
    
    gb_properties->setLayout(propertiesLayout);
    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addLayout(topRowEdit, 0, 0, 1, 2);
    controlsLayout->addWidget(gb_properties, 1, 1, 4, 1);
    
    m_gbEdit->setLayout(controlsLayout);
    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addLayout(topHLayout, 0, 0, 1, 4);
    masterLayout->addWidget(m_gbEdit, 1, 0, 4, 4);

    setLayout(masterLayout);
}
bool PropellantTab::LoadDatabase(const QString& filename)
{
    m_DatabaseFileName = filename;
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
            m_cbPropSelection->addItem(prop.GetPropellantName());
        }
        file.close();
        return true;
    }
    return false;
}
bool PropellantTab::SaveDatabase()
{
    QFile file(m_DatabaseFileName);
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
void PropellantTab::OnPropellantComboBoxIndexChanged(int idx)
{
    if (idx > -1) //is the group box selection valid?
    {
        m_gbEdit->setEnabled(true); 
        m_btnDeleteProp->setEnabled(true);

        OpenBurnPropellant propellant = (*m_Propellants)[idx];
        m_cbPropSelection->setCurrentIndex(idx);
        m_lnePropName->setText(propellant.GetPropellantName());
        m_lnePropBRCoef->setText(QString::number(propellant.GetBurnRateCoef()));
        m_lnePropBRExp->setText(QString::number(propellant.GetBurnRateExp()));
        m_lnePropDensity->setText(QString::number(propellant.GetDensity()));
        m_lnePropCStar->setText(QString::number(propellant.GetCharVelocity()));
        m_lnePropGasSpecificHeatRatio->setText(QString::number(propellant.GetSpecificHeatRatio()));
    }
}
void PropellantTab::ConnectLineEditSignals()
{
    //all of these signals fire the "update propellant" signal
    connect(m_lnePropName, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
    connect(m_lnePropBRCoef, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
    connect(m_lnePropBRExp, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
    connect(m_lnePropDensity, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
    connect(m_lnePropCStar, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
    connect(m_lnePropGasSpecificHeatRatio, SIGNAL(editingFinished()), this, SLOT(OnPropellantUpdated()));
}
void PropellantTab::OnPropellantUpdated()
{
    int idx = m_cbPropSelection->currentIndex() > 0 ? m_cbPropSelection->currentIndex() : 0;
    OpenBurnPropellant& prop = (*m_Propellants)[idx];
    m_cbPropSelection->setItemText(idx, m_lnePropName->text());    
    prop.SetPropellantName(m_lnePropName->text());
    prop.SetBasicParams(
        m_lnePropBRCoef->text().toDouble(),
        m_lnePropBRExp->text().toDouble(),
        m_lnePropCStar->text().toDouble(),
        m_lnePropDensity->text().toDouble(),
        m_lnePropGasSpecificHeatRatio->text().toDouble()
    );
}
void PropellantTab::OnSaveButtonClicked()
{
    qDebug() << "Saving database!";
    OnPropellantUpdated();
    if (SaveDatabase())
    {
        emit PropellantsUpdated();
    }
}
void PropellantTab::OnDeleteButtonClicked()
{
    int oldIndex = m_cbPropSelection->currentIndex() > 0 ? m_cbPropSelection->currentIndex() : 0;    
    QMessageBox::StandardButton resBtn =
        QMessageBox::question( this, "OpenBurn", tr("Are you sure you want to delete propellant: ")
         + (*m_Propellants)[oldIndex].GetPropellantName(),
        QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    
    if (resBtn == QMessageBox::Yes)
    {
        m_Propellants->erase(m_Propellants->begin() + oldIndex);
        m_cbPropSelection->setCurrentIndex(oldIndex);
        m_cbPropSelection->removeItem(oldIndex);
        if (m_Propellants->empty())
        {
            m_gbEdit->setEnabled(false);
            m_btnDeleteProp->setEnabled(false);
        }
        SaveDatabase();    
        }
    else
    {
        return;
    }
}
void PropellantTab::OnNewButtonClicked()
{
    OpenBurnPropellant prop(DEFAULT_NAME);
    m_Propellants->push_back(prop);
    m_cbPropSelection->addItem(DEFAULT_NAME);
    m_cbPropSelection->setCurrentIndex(int(m_Propellants->size()) - 1);
    SetDefaultValues();

    m_gbEdit->setEnabled(true);
    m_btnDeleteProp->setEnabled(true);
}
void PropellantTab::SetDefaultValues()
{
    m_lnePropName->setText(DEFAULT_NAME);  
    m_lnePropBRCoef->setText("0");
    m_lnePropBRExp->setText("0");
    m_lnePropCStar->setText("0");
    m_lnePropDensity->setText("0");
    m_lnePropGasSpecificHeatRatio->setText("1.25");    
}
