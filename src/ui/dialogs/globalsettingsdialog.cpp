#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

#include "globalsettingsdialog.h"

GlobalSettingsDialog::GlobalSettingsDialog(OpenBurnSettings* settings, QWidget* parent)
    : QDialog(parent),
      m_GlobalSettings(settings)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (settings)
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
    unitsLayout->addWidget(new QLabel(tr("Angle")), 1, 0);
    unitsLayout->addWidget(m_unitsAngle = new AngleUnitsComboBox, 1, 1);
    unitsLayout->addWidget(new QLabel(tr("Pressure")), 2, 0);
    unitsLayout->addWidget(m_unitsPressure = new PressureUnitsComboBox, 2, 1);
    unitsLayout->addWidget(new QLabel(tr("Temperature")), 3, 0);
    unitsLayout->addWidget(m_unitsTemperature = new TemperatureUnitsComboBox, 3, 1);
    unitsLayout->addWidget(new QLabel(tr("Force")), 4, 0);
    unitsLayout->addWidget(m_unitsForce = new ForceUnitsComboBox, 4, 1);
    unitsLayout->addWidget(new QLabel(tr("Mass")), 5, 0);
    unitsLayout->addWidget(m_unitsMass = new MassUnitsComboBox, 5, 1);
    unitsLayout->addWidget(m_btnDefaultUnits = new QPushButton(tr("Default")), 255, 0);
    unitsLayout->addWidget(m_btnMetricUnits = new QPushButton(tr("Metric")), 255, 1);
    unitsLayout->addWidget(m_btnImperialUnits = new QPushButton(tr("Imperial")), 255, 2);

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
    if (m_GlobalSettings)
    {
        m_GlobalSettings->m_LengthUnits = m_unitsLength->GetCurrentUnits().unit;
        m_GlobalSettings->m_AngleUnits = m_unitsAngle->GetCurrentUnits().unit;
        m_GlobalSettings->m_PressureUnits = m_unitsPressure->GetCurrentUnits().unit;
        m_GlobalSettings->m_TemperatureUnits = m_unitsTemperature->GetCurrentUnits().unit;
        m_GlobalSettings->m_ForceUnits = m_unitsForce->GetCurrentUnits().unit;
        m_GlobalSettings->m_MassUnits = m_unitsMass->GetCurrentUnits().unit;
        m_GlobalSettings->m_redrawOnChanges = m_chbxReSimOnChanges->isChecked();
        emit m_GlobalSettings->SettingsChanged();
    }
}
void GlobalSettingsDialog::PopulateSettings(SETTINGS_POPULATE_TYPE type)
{
    if (m_GlobalSettings)
    {
        m_chbxReSimOnChanges->setChecked(m_GlobalSettings->m_redrawOnChanges);
        if (type == SETTINGS_POPULATE_TYPE::current)
        {
            m_unitsLength->setCurrentIndex(int(m_GlobalSettings->m_LengthUnits));
            m_unitsAngle->setCurrentIndex(int(m_GlobalSettings->m_AngleUnits));
            m_unitsPressure->setCurrentIndex(int(m_GlobalSettings->m_PressureUnits));
            m_unitsTemperature->setCurrentIndex(int(m_GlobalSettings->m_TemperatureUnits));
            m_unitsForce->setCurrentIndex(int(m_GlobalSettings->m_ForceUnits));
            m_unitsMass->setCurrentIndex(int(m_GlobalSettings->m_MassUnits));
        }
        else if (type == SETTINGS_POPULATE_TYPE::def)
        {
            m_unitsLength->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::inches));
            m_unitsAngle->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_unitsPressure->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::psi));
            m_unitsTemperature->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::fahrenheit));
            m_unitsForce->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::newtons));
            m_unitsMass->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::pounds_mass));
        }
        else if (type == SETTINGS_POPULATE_TYPE::metric)
        {
            m_unitsLength->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::millimeters));
            m_unitsAngle->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_unitsPressure->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::kilopascals));
            m_unitsTemperature->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::kelvin));
            m_unitsForce->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::newtons));
            m_unitsMass->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::kilograms));
        }
        else if (type == SETTINGS_POPULATE_TYPE::imperial)
        {
            m_unitsLength->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::inches));
            m_unitsAngle->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_unitsPressure->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::psi));
            m_unitsTemperature->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::fahrenheit));
            m_unitsForce->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::pounds_force));
            m_unitsMass->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::pounds_mass));
        }
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
