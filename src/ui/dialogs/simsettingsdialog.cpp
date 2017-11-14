#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include "simsettingsdialog.h"

#include "src/units.h"
SimSettingsDialog::SimSettingsDialog(MotorSimSettings* settings, QWidget* parent)
    : QDialog(parent), m_Settings(settings)
{
    SetupUI();
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(ApplyButton_Clicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(CancelButton_Clicked()));
}
SimSettingsDialog::~SimSettingsDialog()
{

}
void SimSettingsDialog::SetupUI()
{
    QGroupBox* box = new QGroupBox(tr("Simulation Settings"));
    QGridLayout* layout = new QGridLayout;
    box->setLayout(layout);

    layout->addWidget(new QLabel(tr("Ambient Pressure")), 0, 0);
    layout->addWidget(m_sb_ambientPressure = new QDoubleSpinBox, 0, 1);
    m_sb_ambientPressure->setValue(14.7);
    m_sb_ambientPressure->setSingleStep(0.1f);
    m_sb_ambientPressure->setMinimum(0.0f);
    m_cb_ambientPressureUnits = new QComboBox;
    m_cb_ambientPressureUnits->addItems(OpenBurnUnits::g_kPressureUnits);
    layout->addWidget(m_cb_ambientPressureUnits, 0, 2);

    layout->addWidget(new QLabel(tr("Ambient Temperature")), 1, 0);
    layout->addWidget(m_sb_ambientTemp = new QDoubleSpinBox, 1, 1);
    m_sb_ambientTemp->setValue(70.0f);
    m_sb_ambientTemp->setSingleStep(0.1f);
    m_sb_ambientTemp->setMinimum(0.0f);
    m_cb_ambientTempUnits = new QComboBox;
    m_cb_ambientTempUnits->addItems(OpenBurnUnits::g_kTemperatureUnits);
    layout->addWidget(m_cb_ambientTempUnits, 1, 2);

    layout->addWidget(new QLabel(tr("Two-phase flow efficiency (%)")), 2, 0);
    layout->addWidget(m_sb_twoPhaseFlow = new QDoubleSpinBox, 2, 1);
    m_sb_twoPhaseFlow->setValue(85.0f);
    m_sb_twoPhaseFlow->setMaximum(100.0f);
    m_sb_twoPhaseFlow->setMinimum(0.0f);
    m_sb_twoPhaseFlow->setSingleStep(0.25f);

    layout->addWidget(new QLabel(tr("Nozzle skin friction efficiency (%)")), 3, 0);
    layout->addWidget(m_sb_skinFriction = new QDoubleSpinBox, 3, 1);
    m_sb_skinFriction->setValue(98.0f);
    m_sb_skinFriction->setMaximum(100.0f);
    m_sb_skinFriction->setMinimum(0.0f);
    m_sb_skinFriction->setSingleStep(0.1f);

    layout->addWidget(new QLabel(tr("Simulation time step")), 4, 0);
    layout->addWidget(m_sb_timeStep = new QDoubleSpinBox, 4, 1);
    m_sb_timeStep->setValue(0.01f);
    m_sb_timeStep->setDecimals(4);
    m_sb_timeStep->setMaximum(0.25);
    m_sb_timeStep->setMinimum(0.001f);
    m_sb_timeStep->setSingleStep(0.001f);

    m_applyButton = new QPushButton(tr("Apply"), this);
    m_cancelButton = new QPushButton(tr("Close"), this);

    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addWidget(box, 0, 0, 1, 2);
    masterLayout->addWidget(m_applyButton, 255, 0);        
    masterLayout->addWidget(m_cancelButton, 255, 1);

    setLayout(masterLayout);
}
void SimSettingsDialog::ApplySettings()
{
    if (m_Settings)
    {
        m_Settings->ambientPressure = m_sb_ambientPressure->value();
        m_Settings->ambientTemp = m_sb_ambientTemp->value();
        m_Settings->twoPhaseFlowEfficency = m_sb_twoPhaseFlow->value() * 0.01f; //precent
        m_Settings->skinFrictionEfficency = m_sb_skinFriction->value() * 0.01f; //percent
        m_Settings->timeStep = m_sb_timeStep->value();
    }
}
void SimSettingsDialog::ApplyButton_Clicked()
{
    ApplySettings();
}
void SimSettingsDialog::CancelButton_Clicked()
{
    close();
}