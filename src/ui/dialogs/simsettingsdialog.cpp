#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include "simsettingsdialog.hpp"

#include "src/units.hpp"
SimSettingsDialog::SimSettingsDialog(OpenBurnApplication& app, QWidget* parent)
    : QDialog(parent),
      m_app(app)
{
    SetupUI();
    connect(m_btnApply, &QPushButton::clicked,
            this, &SimSettingsDialog::OnApplyButtonClicked);
    connect(m_btnCancel, &QPushButton::clicked,
            this, &SimSettingsDialog::OnCancelButtonClicked);
    m_unitsAmbientPressure->SetUnits(m_app.GetGlobalSettings().m_PressureUnits);
    m_unitsAmbientTemp->SetUnits(m_app.GetGlobalSettings().m_TemperatureUnits);

}
void SimSettingsDialog::SetupUI()
{
    QGroupBox* box = new QGroupBox(tr("Simulation Settings"));
    QGridLayout* layout = new QGridLayout;
    box->setLayout(layout);

    layout->addWidget(new QLabel(tr("Ambient Pressure")), 0, 0);
    layout->addWidget(m_sbAmbientPressure = new QDoubleSpinBox, 0, 1);
    m_sbAmbientPressure->setSingleStep(0.1f);
    m_sbAmbientPressure->setMinimum(0.0f);
    m_sbAmbientPressure->setValue(m_app.GetSimSettings().ambientPressure);
    m_unitsAmbientPressure = new PressureUnitsComboBox(this, m_sbAmbientPressure);
    layout->addWidget(m_unitsAmbientPressure, 0, 2);

    layout->addWidget(new QLabel(tr("Ambient Temperature")), 1, 0);
    layout->addWidget(m_sbAmbientTemp = new QDoubleSpinBox, 1, 1);
    m_sbAmbientTemp->setSingleStep(0.1f);
    m_sbAmbientTemp->setMinimum(0.0f);
    m_sbAmbientTemp->setValue(m_app.GetSimSettings().ambientTemp);
    m_unitsAmbientTemp = new TemperatureUnitsComboBox(this, m_sbAmbientTemp);
    layout->addWidget(m_unitsAmbientTemp, 1, 2);

    layout->addWidget(new QLabel(tr("Two-phase flow efficiency (%)")), 2, 0);
    layout->addWidget(m_sbTwoPhaseFlow = new QDoubleSpinBox, 2, 1);
    m_sbTwoPhaseFlow->setMaximum(100.0f);
    m_sbTwoPhaseFlow->setMinimum(0.0f);
    m_sbTwoPhaseFlow->setSingleStep(0.25f);
    m_sbTwoPhaseFlow->setValue(m_app.GetSimSettings().twoPhaseFlowEfficency * 100.0);

    layout->addWidget(new QLabel(tr("Nozzle skin friction efficiency (%)")), 3, 0);
    layout->addWidget(m_sbSkinFriction = new QDoubleSpinBox, 3, 1);
    m_sbSkinFriction->setMaximum(100.0f);
    m_sbSkinFriction->setMinimum(0.0f);
    m_sbSkinFriction->setSingleStep(0.1f);
    m_sbSkinFriction->setValue(m_app.GetSimSettings().skinFrictionEfficency * 100.0);

    layout->addWidget(new QLabel(tr("Simulation time step")), 4, 0);
    layout->addWidget(m_sbTimeStep = new QDoubleSpinBox, 4, 1);
    m_sbTimeStep->setDecimals(4);
    m_sbTimeStep->setRange(0.001f, 0.25f);
    m_sbTimeStep->setSingleStep(0.001f);
    m_sbTimeStep->setValue(m_app.GetSimSettings().timeStep);

    m_btnApply = new QPushButton(tr("Apply"), this);
    m_btnCancel = new QPushButton(tr("Close"), this);

    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addWidget(box, 0, 0, 1, 2);
    masterLayout->addWidget(m_btnApply, 255, 0);        
    masterLayout->addWidget(m_btnCancel, 255, 1);

    setLayout(masterLayout);
}
void SimSettingsDialog::ApplySettings()
{
    m_app.GetSimSettings().ambientPressure = m_sbAmbientPressure->value();
    m_app.GetSimSettings().ambientTemp = m_sbAmbientTemp->value();
    m_app.GetSimSettings().twoPhaseFlowEfficency = m_sbTwoPhaseFlow->value() * 0.01f; //percent
    m_app.GetSimSettings().skinFrictionEfficency = m_sbSkinFriction->value() * 0.01f; //percent
    m_app.GetSimSettings().timeStep = m_sbTimeStep->value();
    emit m_app.GetSimSettings().SettingsChanged();
}
void SimSettingsDialog::OnApplyButtonClicked()
{
    ApplySettings();
}
void SimSettingsDialog::OnCancelButtonClicked()
{
    close();
}
