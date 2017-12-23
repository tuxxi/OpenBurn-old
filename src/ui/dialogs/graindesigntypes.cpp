#include <QLabel>

#include "src/ui/dialogs/graindesigntypes.hpp"

BaseGrainDesign::BaseGrainDesign(
    QGridLayout* layout,
    OpenBurnGrain* seed,
    OpenBurnSettings* settings)
    : m_grainSeed(seed),
      m_GlobalSettings(settings),
      m_layControls(layout)
{

}
BaseGrainDesign::~BaseGrainDesign()
{
    //remove all the widgets added after row 4 up to row 64
    for (int i = 4; i < 64; ++i)
    {
        //3 columns
        for (int k = 0; k < 3; ++k)
        {
            auto item = m_layControls->itemAtPosition(i, k);
            if (item != nullptr)
            {
                m_layControls->removeItem(item);
                delete item->widget();
            }
        }
    }
}
//BATES
CylindricalGrainDesign::CylindricalGrainDesign(
    QGridLayout* layout,
    OpenBurnGrain* seed ,
    OpenBurnSettings* settings)
    : BaseGrainDesign(layout, seed, settings)
{
    //Grain Core Diameter
    m_layControls->addWidget(new QLabel(tr("Core Diameter")), 4, 0);
    m_layControls->addWidget(m_sbGrainCoreDia = new QDoubleSpinBox, 4, 1);
    m_layControls->addWidget(m_unitsGrainCoreDia = new LengthUnitsComboBox(nullptr, m_sbGrainCoreDia), 4, 2);
    m_unitsGrainCoreDia->setLayoutDirection(Qt::LeftToRight);

    if (m_GlobalSettings)
    {
        m_unitsGrainCoreDia->SetUnits(m_GlobalSettings->m_LengthUnits);
    }
    if (auto bates = dynamic_cast<CylindricalGrain*>(m_grainSeed))
    {
        m_sbGrainCoreDia->setValue(m_unitsGrainCoreDia->GetCurrentUnits().ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            bates->GetCoreDiameter()));
    }

    connect(m_sbGrainCoreDia, SIGNAL(valueChanged(double)), this, SIGNAL(GrainDesignChanged()));
}
double CylindricalGrainDesign::GetCoreDiameter() const
{
    return m_unitsGrainCoreDia->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches, 
        m_sbGrainCoreDia->value());
}
MoonGrainDesign::MoonGrainDesign(QGridLayout *layout, OpenBurnGrain *seed, OpenBurnSettings *settings)
    : BaseGrainDesign(layout, seed, settings)
{
    //Grain Core Diameter
    m_layControls->addWidget(new QLabel(tr("Core Diameter")), 4, 0);
    m_layControls->addWidget(m_sbGrainCoreDia = new QDoubleSpinBox, 4, 1);
    m_layControls->addWidget(m_unitsGrainCoreDia = new LengthUnitsComboBox(nullptr, m_sbGrainCoreDia), 4, 2);
    m_unitsGrainCoreDia->setLayoutDirection(Qt::LeftToRight);

    m_layControls->addWidget(new QLabel(tr("Core Offset")), 5, 0);
    m_layControls->addWidget(m_sbCoreOffset = new QDoubleSpinBox, 5, 1);
    m_layControls->addWidget(m_unitsCoreOffset = new LengthUnitsComboBox(nullptr, m_sbCoreOffset), 5, 2);
    m_unitsCoreOffset->setLayoutDirection(Qt::LeftToRight);

    if (m_GlobalSettings)
    {
        m_unitsGrainCoreDia->SetUnits(m_GlobalSettings->m_LengthUnits);
        m_unitsCoreOffset->SetUnits(m_GlobalSettings->m_LengthUnits);
    }
    if (auto moon = dynamic_cast<MoonGrain*>(m_grainSeed))
    {
        m_sbGrainCoreDia->setValue(m_unitsGrainCoreDia->GetCurrentUnits().ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            moon->GetCoreDiameter()));
        m_sbCoreOffset->setValue(m_unitsGrainCoreDia->GetCurrentUnits().ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            moon->GetCoreOffset()));
    }

    connect(m_sbGrainCoreDia, SIGNAL(valueChanged(double)), this, SIGNAL(GrainDesignChanged()));
    connect(m_sbCoreOffset, SIGNAL(valueChanged(double)), this, SIGNAL(GrainDesignChanged()));
}
double MoonGrainDesign::GetCoreOffset() const
{
    return m_unitsCoreOffset->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches,
        m_sbCoreOffset->value());
}
double MoonGrainDesign::GetCoreDiameter() const
{
    return m_unitsGrainCoreDia->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches,
        m_sbGrainCoreDia->value());
}
