#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

#include "globalsettingsdialog.hpp"

GlobalSettingsDialog::GlobalSettingsDialog(OpenBurnApplication& app, QWidget* parent)
    : QDialog(parent),
      m_app(app)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (m_app.AreSettingsLoaded())
    {
        PopulateSettings(SETTINGS_POPULATE_TYPE::current);
    }
    else
    {
        PopulateSettings(SETTINGS_POPULATE_TYPE::def);
    }
    connect(m_btnCancel, &QPushButton::clicked,
            this, &GlobalSettingsDialog::OnCancelButtonClicked);
    connect(m_btnApply, &QPushButton::clicked,
            this, &GlobalSettingsDialog::OnApplyButtonClicked);
    connect(m_btnDefaultUnits, &QPushButton::clicked,
            this, &GlobalSettingsDialog::OnDefaultUnitsButtonClicked);
    connect(m_btnMetricUnits, &QPushButton::clicked,
            this, &GlobalSettingsDialog::OnMetricUnitsButtonClicked);
    connect(m_btnImperialUnits, &QPushButton::clicked,
            this, &GlobalSettingsDialog::OnImperialUnitsButtonClicked);
}
void GlobalSettingsDialog::SetupUI()
{
    setWindowTitle(tr("OpenBurn Settings"));

    QGroupBox* settingsBox = new QGroupBox("Settings");
    QGridLayout* settingsLayout = new QGridLayout;
    settingsBox->setLayout(settingsLayout);
    settingsLayout->addWidget(new QLabel(tr("Re-simulate when design changes")), 0, 0);
    settingsLayout->addWidget(m_chbxReSimOnChanges = new QCheckBox, 0, 1);

    QGroupBox* unitsBox = new QGroupBox(tr("Units"));
    QGridLayout* unitsLayout = new QGridLayout;
    unitsBox->setLayout(unitsLayout);

    unitsLayout->addWidget(new QLabel(tr("Length")), 0, 0);
    unitsLayout->addWidget(m_unitsLength = new LengthUnitsComboBox, 0, 1);
    unitsLayout->addWidget(new QLabel(tr("Mass")), 1, 0);
    unitsLayout->addWidget(m_unitsMass = new MassUnitsComboBox, 1, 1);
    unitsLayout->addWidget(new QLabel(tr("Force")), 2, 0);
    unitsLayout->addWidget(m_unitsForce = new ForceUnitsComboBox, 2, 1);
    unitsLayout->addWidget(new QLabel(tr("Pressure")), 3, 0);
    unitsLayout->addWidget(m_unitsPressure = new PressureUnitsComboBox, 3, 1);
    unitsLayout->addWidget(new QLabel(tr("Mass Flux")), 4, 0);
    unitsLayout->addWidget(m_unitsMassFlux = new MassFluxUnitsComboBox, 4, 1);
    unitsLayout->addWidget(new QLabel(tr("Velocity")), 5, 0);
    unitsLayout->addWidget(m_unitsVelocity = new VelocityUnitsComboBox, 5, 1);
    unitsLayout->addWidget(new QLabel(tr("Density")), 6, 0);
    unitsLayout->addWidget(m_unitsDensity = new DensityUnitsComboBox, 6, 1);
    unitsLayout->addWidget(new QLabel(tr("Burn Rate")), 7, 0);
    unitsLayout->addWidget(m_unitsBurnRate = new BurnRateUnitsComboBox, 7, 1);
    unitsLayout->addWidget(new QLabel(tr("Temperature")), 8, 0);
    unitsLayout->addWidget(m_unitsTemperature = new TemperatureUnitsComboBox, 8, 1);
    unitsLayout->addWidget(new QLabel(tr("Angle")), 9, 0);
    unitsLayout->addWidget(m_unitsAngle = new AngleUnitsComboBox, 9, 1);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_btnDefaultUnits = new QPushButton(tr("Default")));
    buttonsLayout->addWidget(m_btnMetricUnits = new QPushButton(tr("Metric")));
    buttonsLayout->addWidget(m_btnImperialUnits = new QPushButton(tr("Imperial")));
    unitsLayout->addLayout(buttonsLayout, 255, 0, 1, 3);

    m_btnApply = new QPushButton(tr("Apply"), this);
    m_btnCancel = new QPushButton(tr("Close"), this);

    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addWidget(settingsBox, 0, 0, 1, 2);
    masterLayout->addWidget(unitsBox, 1, 0, 1, 2);
    masterLayout->addWidget(m_btnApply, 255, 0);        
    masterLayout->addWidget(m_btnCancel, 255, 1);

    setLayout(masterLayout);
}
void GlobalSettingsDialog::ApplySettings()
{
    m_app.GetGlobalSettings().m_LengthUnits = m_unitsLength->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_AngleUnits = m_unitsAngle->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_PressureUnits = m_unitsPressure->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_TemperatureUnits = m_unitsTemperature->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_ForceUnits = m_unitsForce->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_MassUnits = m_unitsMass->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_VelocityUnits = m_unitsVelocity->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_MassFluxUnits = m_unitsMassFlux->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_DensityUnits = m_unitsDensity->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_BurnRateUnits = m_unitsBurnRate->GetCurrentUnits().unit;
    m_app.GetGlobalSettings().m_redrawOnChanges = m_chbxReSimOnChanges->isChecked();
    emit m_app.GetGlobalSettings().SettingsChanged();

}
void GlobalSettingsDialog::PopulateSettings(SETTINGS_POPULATE_TYPE type)
{
    m_chbxReSimOnChanges->setChecked(m_app.GetGlobalSettings().m_redrawOnChanges);
    if (type == SETTINGS_POPULATE_TYPE::current)
    {
        m_unitsLength->SetUnits(m_app.GetGlobalSettings().m_LengthUnits);
        m_unitsAngle->SetUnits(m_app.GetGlobalSettings().m_AngleUnits);
        m_unitsPressure->SetUnits(m_app.GetGlobalSettings().m_PressureUnits);
        m_unitsTemperature->SetUnits(m_app.GetGlobalSettings().m_TemperatureUnits);
        m_unitsForce->SetUnits(m_app.GetGlobalSettings().m_ForceUnits);
        m_unitsMass->SetUnits(m_app.GetGlobalSettings().m_MassUnits);
        m_unitsVelocity->SetUnits(m_app.GetGlobalSettings().m_VelocityUnits);
        m_unitsMassFlux->SetUnits(m_app.GetGlobalSettings().m_MassFluxUnits);
        m_unitsDensity->SetUnits(m_app.GetGlobalSettings().m_DensityUnits);
        m_unitsBurnRate->SetUnits(m_app.GetGlobalSettings().m_BurnRateUnits);
    }
    else if (type == SETTINGS_POPULATE_TYPE::def)
    {
        m_unitsLength->SetUnits(OpenBurnUnits::LengthUnits_T::inches);
        m_unitsAngle->SetUnits(OpenBurnUnits::AngleUnits_T::degrees);
        m_unitsPressure->SetUnits(OpenBurnUnits::PressureUnits_T::psi);
        m_unitsTemperature->SetUnits(OpenBurnUnits::TemperatureUnits_T::fahrenheit);
        m_unitsForce->SetUnits(OpenBurnUnits::ForceUnits_T::newtons);
        m_unitsMass->SetUnits(OpenBurnUnits::MassUnits_T::pounds_mass);
        m_unitsVelocity->SetUnits(OpenBurnUnits::VelocityUnits_T::feet_per_second);
        m_unitsMassFlux->SetUnits(OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in);
        m_unitsDensity->SetUnits(OpenBurnUnits::DensityUnits_T::lbs_per_in_cu);
        m_unitsBurnRate->SetUnits(OpenBurnUnits::BurnRateUnits_T::inches_per_second);
    }
    else if (type == SETTINGS_POPULATE_TYPE::metric)
    {
        m_unitsLength->SetUnits(OpenBurnUnits::LengthUnits_T::millimeters);
        m_unitsAngle->SetUnits(OpenBurnUnits::AngleUnits_T::degrees);
        m_unitsPressure->SetUnits(OpenBurnUnits::PressureUnits_T::kilopascals);
        m_unitsTemperature->SetUnits(OpenBurnUnits::TemperatureUnits_T::kelvin);
        m_unitsForce->SetUnits(OpenBurnUnits::ForceUnits_T::newtons);
        m_unitsMass->SetUnits(OpenBurnUnits::MassUnits_T::kilograms);
        m_unitsVelocity->SetUnits(OpenBurnUnits::VelocityUnits_T::meters_per_second);
        m_unitsMassFlux->SetUnits(OpenBurnUnits::MassFluxUnits_T::kg_sec_sq_meter);
        m_unitsDensity->SetUnits(OpenBurnUnits::DensityUnits_T::kg_per_m_cu);
        m_unitsBurnRate->SetUnits(OpenBurnUnits::BurnRateUnits_T::millimeters_per_second);

    }
    else if (type == SETTINGS_POPULATE_TYPE::imperial)
    {
        m_unitsLength->SetUnits(OpenBurnUnits::LengthUnits_T::inches);
        m_unitsAngle->SetUnits(OpenBurnUnits::AngleUnits_T::degrees);
        m_unitsPressure->SetUnits(OpenBurnUnits::PressureUnits_T::psi);
        m_unitsTemperature->SetUnits(OpenBurnUnits::TemperatureUnits_T::fahrenheit);
        m_unitsForce->SetUnits(OpenBurnUnits::ForceUnits_T::pounds_force);
        m_unitsMass->SetUnits(OpenBurnUnits::MassUnits_T::pounds_mass);
        m_unitsVelocity->SetUnits(OpenBurnUnits::VelocityUnits_T::feet_per_second);
        m_unitsMassFlux->SetUnits(OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in);
        m_unitsDensity->SetUnits(OpenBurnUnits::DensityUnits_T::slugs_per_foot_cu);
        m_unitsBurnRate->SetUnits(OpenBurnUnits::BurnRateUnits_T::inches_per_second);
    }
}
void GlobalSettingsDialog::OnApplyButtonClicked()
{
    ApplySettings();
}
void GlobalSettingsDialog::OnCancelButtonClicked()
{
    close();
}
void GlobalSettingsDialog::OnDefaultUnitsButtonClicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::def);
}
void GlobalSettingsDialog::OnMetricUnitsButtonClicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::metric);
}
void GlobalSettingsDialog::OnImperialUnitsButtonClicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::imperial);
}
