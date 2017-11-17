#include <QLabel>
#include <QDebug>

#include "src/ui/dialogs/graindesigntypes.h"
#include "src/units.h"

OpenBurnDesignGrain::OpenBurnDesignGrain(
    PropellantList* prop,
    OpenBurnGrain* seed,
    OpenBurnSettings* settings,
    QWidget* parent)
    : QWidget(parent), m_seedGrain(seed), m_GlobalSettings(settings), m_Propellants(prop)
{
    SetupUI();
    connect(m_grainTypeComboBox, SIGNAL(currentIndexChanged(int)), 
        this, SLOT(on_grainType_changed(int)));
    connect(m_grainDiameterSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(SIG_GrainDesign_Changed()));
    connect(m_grainLengthSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(SIG_GrainDesign_Changed()));
    connect(m_grainInhibitedFacesSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(SIG_GrainDesign_Changed()));
    
    SeedValues();
}
void OpenBurnDesignGrain::SetupUI()
{
    controlsLayout = new QGridLayout;
    //Grain type combo box
    m_grainTypeComboBox = new QComboBox(this);
    m_grainTypeComboBox->addItems(QStringList() 
        << tr("Cylindrical (BATES)"));
    QLabel* label_0 = new QLabel(tr("Grain Core Type"), this);
    controlsLayout->addWidget(label_0, 0, 0);
    controlsLayout->addWidget(m_grainTypeComboBox, 0, 1);

    //Propellant Selection Box 
    m_propellantComboBox = new QComboBox(this);
    QLabel* label = new QLabel(tr("Propellant Type"), this);
    if (m_Propellants)
    {
        for (auto i : *m_Propellants)
        {
            m_propellantComboBox->addItem(i.GetPropellantName());            
        }
    }
    m_modifyPropellantDatabase = new QToolButton(this);
    m_modifyPropellantDatabase->setText("...");    
    controlsLayout->addWidget(label, 1, 0);
    controlsLayout->addWidget(m_propellantComboBox, 1, 1);
    controlsLayout->addWidget(m_modifyPropellantDatabase, 1, 2);

    //Grain Length 
    m_grainLengthSpinBox = new QDoubleSpinBox(this);
    m_grainLengthSpinBox->setDecimals(3);
    m_grainLengthSpinBox->setSingleStep(0.25);
    QLabel* label_2 = new QLabel(tr("Grain Length"), this);
    m_grainLenUnitsComboBox = new LengthUnitsComboBox(this, m_grainLengthSpinBox);
    m_grainLenUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    controlsLayout->addWidget(label_2, 2, 0);
    controlsLayout->addWidget(m_grainLengthSpinBox, 2, 1);
    controlsLayout->addWidget(m_grainLenUnitsComboBox, 2, 2);

    //Grain Diameter
    m_grainDiameterSpinBox = new QDoubleSpinBox(this);
    m_grainDiameterSpinBox->setDecimals(3);
    m_grainDiameterSpinBox->setSingleStep(0.25);
    QLabel* label_3 = new QLabel(tr("Grain Diameter"), this);
    m_grainDiaUnitsComboBox = new LengthUnitsComboBox(this, m_grainDiameterSpinBox);
    m_grainDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    controlsLayout->addWidget(label_3, 3, 0);
    controlsLayout->addWidget(m_grainDiameterSpinBox, 3, 1);
    controlsLayout->addWidget(m_grainDiaUnitsComboBox, 3, 2);

    if (m_GlobalSettings)
    {
        m_grainDiaUnitsComboBox->SetUnits(m_GlobalSettings->m_LengthUnits);
        m_grainLenUnitsComboBox->SetUnits(m_GlobalSettings->m_LengthUnits);
    }

    //Inhibited Faces
    m_grainInhibitedFacesSpinBox = new QSpinBox(this);
    m_grainInhibitedFacesSpinBox->setMaximum(2);
    QLabel* label_5 = new QLabel(tr("Inhibited Faces"), this);

    controlsLayout->addWidget(label_5, 255, 0);
    controlsLayout->addWidget(m_grainInhibitedFacesSpinBox, 255, 1);    

    setTabOrder(m_grainTypeComboBox, m_propellantComboBox);
    setTabOrder(m_propellantComboBox, m_grainLengthSpinBox);
    setTabOrder(m_grainLengthSpinBox, m_grainDiameterSpinBox);
    setTabOrder(m_grainDiameterSpinBox, m_grainInhibitedFacesSpinBox);
    
    setLayout(controlsLayout);
}
OpenBurnDesignGrain::~OpenBurnDesignGrain()
{

}
void OpenBurnDesignGrain::SeedValues()
{
    if (m_seedGrain)
    {
        m_grainLengthSpinBox->setValue(m_seedGrain->GetLength());
        m_grainDiameterSpinBox->setValue(m_seedGrain->GetDiameter());
        m_grainInhibitedFacesSpinBox->setValue(m_seedGrain->GetInhibitedFaces());
    
        //propellant ..     
    }
}
double OpenBurnDesignGrain::GetLength()
{
    return OpenBurnUnits::ConvertLength(
        m_grainLenUnitsComboBox->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_grainLengthSpinBox->value());
}
double OpenBurnDesignGrain::GetDiameter()
{
    return OpenBurnUnits::ConvertLength(
        m_grainDiaUnitsComboBox->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_grainDiameterSpinBox->value());
}
int OpenBurnDesignGrain::GetInhibitedFaces()
{
    return m_grainInhibitedFacesSpinBox->value();
}
GRAINTYPE OpenBurnDesignGrain::GetGrainType()
{
    return static_cast<GRAINTYPE>(m_grainTypeComboBox->currentIndex());
}
OpenBurnPropellant OpenBurnDesignGrain::GetPropellant()
{
    if (!m_Propellants->empty())
    {
        int idx = m_propellantComboBox->currentIndex() > 0 ? m_propellantComboBox->currentIndex() : 0;
        return (*m_Propellants)[idx];
    }
    return OpenBurnPropellant();
}

void OpenBurnDesignGrain::on_grainType_changed(int idx)
{
    //TODO: for some reason, the value in the grain design's type spin box doesn't update even though it says the index updated...
    emit SIG_GrainType_Changed(static_cast<GRAINTYPE>(idx));
}
void OpenBurnDesignGrain::AddNewControls(QWidget* widet, int row, int col)
{
    controlsLayout->addWidget(widet, row+4, col);
}
//BATES
BatesGrainDesign::BatesGrainDesign(
    PropellantList* prop, 
    OpenBurnGrain* seed , 
    OpenBurnSettings* settings,
    QWidget* parent)
    : OpenBurnDesignGrain(prop, seed, settings, parent)
{
    //Grain Core Diameter
    m_grainCoreDiameterSpinBox = new QDoubleSpinBox(this);
    m_grainCoreDiameterSpinBox->setDecimals(3);
    m_grainCoreDiameterSpinBox->setSingleStep(0.25);
    QLabel* label_4 = new QLabel(tr("Grain Core Diameter"), this);
    m_grainCoreDiaUnitsComboBox = new LengthUnitsComboBox(this, m_grainCoreDiameterSpinBox);
    m_grainCoreDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    if (m_GlobalSettings)
    {
        m_grainCoreDiaUnitsComboBox->SetUnits(m_GlobalSettings->m_LengthUnits);
    }

    AddNewControls(label_4, 0, 0);
    AddNewControls(m_grainCoreDiameterSpinBox, 0, 1);
    AddNewControls(m_grainCoreDiaUnitsComboBox, 0, 2);
    
    setTabOrder(m_grainDiameterSpinBox, m_grainCoreDiameterSpinBox);        
    setTabOrder(m_grainCoreDiameterSpinBox, m_grainInhibitedFacesSpinBox);

    SeedValues();
    connect(m_grainCoreDiameterSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(SIG_GrainDesign_Changed()));
}
BatesGrainDesign::~BatesGrainDesign()
{

}
double BatesGrainDesign::GetCoreDiameter()
{
    return OpenBurnUnits::ConvertLength(
        m_grainCoreDiaUnitsComboBox->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_grainCoreDiameterSpinBox->value());
}
void BatesGrainDesign::SeedValues()
{
    OpenBurnDesignGrain::SeedValues();
    if (BatesGrain* seed = dynamic_cast<BatesGrain*>(m_seedGrain))
    {
        m_grainCoreDiameterSpinBox->setValue(seed->GetCoreDiameter());
    }
}
