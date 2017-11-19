#include <QLabel>
#include <QDebug>

#include "src/ui/dialogs/graindesigntypes.h"
#include "src/units.h"

OpenBurnDesignGrain::OpenBurnDesignGrain(PropellantList* prop, OpenBurnGrain* seed,
                                         OpenBurnSettings* settings, QWidget* parent)
    : QWidget(parent),
      m_grainSeed(seed),
      m_GlobalSettings(settings),
      m_Propellants(prop)
{
    SetupUI();
    SeedValues();
    //new function pointer syntax does not work for overloaded signals or slots >.<
    connect(m_sbGrainDia, SIGNAL(valueChanged(double)),
            this, SIGNAL(GrainDesignChanged()));
    connect(m_sbGrainLen, SIGNAL(valueChanged(double)),
            this, SIGNAL(GrainDesignChanged()));
    connect(m_sbGrainInhibit, SIGNAL(valueChanged(int)),
            this, SIGNAL(GrainDesignChanged()));
    
}
void OpenBurnDesignGrain::SetupUI()
{
    m_layControls = new QGridLayout;
    //Grain type combo box
    m_cbGrainType = new QComboBox(this);
    m_cbGrainType->addItems(QStringList() 
        << tr("Cylindrical (BATES)"));
    QLabel* label_0 = new QLabel(tr("Grain Core Type"), this);
    m_layControls->addWidget(label_0, 0, 0);
    m_layControls->addWidget(m_cbGrainType, 0, 1);

    //Propellant Selection Box 
    m_cbPropellantType = new QComboBox(this);
    QLabel* label = new QLabel(tr("Propellant Type"), this);
    if (m_Propellants)
    {
        for (auto i : *m_Propellants)
        {
            m_cbPropellantType->addItem(i.GetPropellantName());            
        }
    }
    m_btntModifyPropellant = new QToolButton(this);
    m_btntModifyPropellant->setText("...");    
    m_layControls->addWidget(label, 1, 0);
    m_layControls->addWidget(m_cbPropellantType, 1, 1);
    m_layControls->addWidget(m_btntModifyPropellant, 1, 2);

    //Grain Length 
    m_sbGrainLen = new QDoubleSpinBox(this);
    QLabel* label_2 = new QLabel(tr("Grain Length"), this);
    m_unitsGrainLen = new LengthUnitsComboBox(this, m_sbGrainLen);
    m_unitsGrainLen->setLayoutDirection(Qt::LeftToRight);
    m_layControls->addWidget(label_2, 2, 0);
    m_layControls->addWidget(m_sbGrainLen, 2, 1);
    m_layControls->addWidget(m_unitsGrainLen, 2, 2);

    //Grain Diameter
    m_sbGrainDia = new QDoubleSpinBox(this);
    QLabel* label_3 = new QLabel(tr("Grain Diameter"), this);
    m_unitsGrainDia = new LengthUnitsComboBox(this, m_sbGrainDia);
    m_unitsGrainDia->setLayoutDirection(Qt::LeftToRight);
    m_layControls->addWidget(label_3, 3, 0);
    m_layControls->addWidget(m_sbGrainDia, 3, 1);
    m_layControls->addWidget(m_unitsGrainDia, 3, 2);

    if (m_GlobalSettings)
    {
        m_unitsGrainDia->SetUnits(m_GlobalSettings->m_LengthUnits);
        m_unitsGrainLen->SetUnits(m_GlobalSettings->m_LengthUnits);
    }

    //Inhibited Faces
    m_sbGrainInhibit = new QSpinBox(this);
    m_sbGrainInhibit->setMaximum(2);
    QLabel* label_5 = new QLabel(tr("Inhibited Faces"), this);

    m_layControls->addWidget(label_5, 255, 0);
    m_layControls->addWidget(m_sbGrainInhibit, 255, 1);    

    setTabOrder(m_cbGrainType, m_cbPropellantType);
    setTabOrder(m_cbPropellantType, m_sbGrainLen);
    setTabOrder(m_sbGrainLen, m_sbGrainDia);
    setTabOrder(m_sbGrainDia, m_sbGrainInhibit);
    
    setLayout(m_layControls);
}
void OpenBurnDesignGrain::SeedValues()
{
    if (m_grainSeed)
    {
        m_sbGrainLen->setValue(m_grainSeed->GetLength());
        m_sbGrainDia->setValue(m_grainSeed->GetDiameter());
        m_sbGrainInhibit->setValue(m_grainSeed->GetInhibitedFaces());
    
        //propellant ..     
    }
}
double OpenBurnDesignGrain::GetLength()
{
    return OpenBurnUnits::ConvertLength(
        m_unitsGrainLen->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_sbGrainLen->value());
}
double OpenBurnDesignGrain::GetDiameter()
{
    return OpenBurnUnits::ConvertLength(
        m_unitsGrainDia->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_sbGrainDia->value());
}
int OpenBurnDesignGrain::GetInhibitedFaces()
{
    return m_sbGrainInhibit->value();
}
OpenBurnPropellant OpenBurnDesignGrain::GetPropellant()
{
    if (!m_Propellants->empty())
    {
        int idx = m_cbPropellantType->currentIndex() > 0 ? m_cbPropellantType->currentIndex() : 0;
        return (*m_Propellants)[idx];
    }
    return OpenBurnPropellant();
}
void OpenBurnDesignGrain::AddNewControls(QWidget* widet, int row, int col)
{
    m_layControls->addWidget(widet, row+4, col);
}
//BATES
CylindricalGrainDesign::CylindricalGrainDesign(
    PropellantList* prop, 
    OpenBurnGrain* seed , 
    OpenBurnSettings* settings,
    QWidget* parent)
    : OpenBurnDesignGrain(prop, seed, settings, parent)
{
    //Grain Core Diameter
    m_sbGrainCoreDia = new QDoubleSpinBox(this);
    QLabel* label_4 = new QLabel(tr("Grain Core Diameter"), this);
    m_unitsGrainCoreDia = new LengthUnitsComboBox(this, m_sbGrainCoreDia);
    m_unitsGrainCoreDia->setLayoutDirection(Qt::LeftToRight);
    if (m_GlobalSettings)
    {
        m_unitsGrainCoreDia->SetUnits(m_GlobalSettings->m_LengthUnits);
    }

    AddNewControls(label_4, 0, 0);
    AddNewControls(m_sbGrainCoreDia, 0, 1);
    AddNewControls(m_unitsGrainCoreDia, 0, 2);
    
    setTabOrder(m_sbGrainDia, m_sbGrainCoreDia);        
    setTabOrder(m_sbGrainCoreDia, m_sbGrainInhibit);

    SeedValues();
    connect(m_sbGrainCoreDia, SIGNAL(valueChanged(double)), this, SIGNAL(GrainDesignChanged()));
}
double CylindricalGrainDesign::GetCoreDiameter()
{
    return OpenBurnUnits::ConvertLength(
        m_unitsGrainCoreDia->GetCurrentUnits(),
        OpenBurnUnits::LengthUnits_T::inches, 
        m_sbGrainCoreDia->value());
}
void CylindricalGrainDesign::SeedValues()
{
    OpenBurnDesignGrain::SeedValues();
    if (CylindricalGrain* seed = dynamic_cast<CylindricalGrain*>(m_grainSeed))
    {
        m_sbGrainCoreDia->setValue(seed->GetCoreDiameter());
    }
}
