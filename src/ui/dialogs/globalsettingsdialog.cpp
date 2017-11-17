#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

#include "globalsettingsdialog.h"

GlobalSettingsDialog::GlobalSettingsDialog(OpenBurnSettings* settings, QWidget* parent)
    : QDialog(parent), m_GlobalSettings(settings)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (settings)
    {
        PopulateSettings(SETTINGS_POPULATE_TYPE::current_);
    }
    else
    {
        PopulateSettings(SETTINGS_POPULATE_TYPE::default_);
    }
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(CancelButton_Clicked()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(ApplyButton_Clicked()));
    connect(m_defaultUnitsButton, SIGNAL(clicked()), this, SLOT(DefaultUnitsButton_Clicked()));
    connect(m_metricUnitsButton, SIGNAL(clicked()), this, SLOT(MetricUnitsButton_Clicked()));
    connect(m_imperialUnitsButton, SIGNAL(clicked()), this, SLOT(ImperialUnitsButton_Clicked()));
}
GlobalSettingsDialog::~GlobalSettingsDialog() {}
void GlobalSettingsDialog::SetupUI()
{
    setWindowTitle(tr("OpenBurn Settings"));

    QGroupBox* settingsBox = new QGroupBox("Settings");
    QGridLayout* settingsLayout = new QGridLayout;
    settingsBox->setLayout(settingsLayout);
    settingsLayout->addWidget(new QLabel(tr("Re-simulate when design changes")), 0, 0);
    settingsLayout->addWidget(m_reSimCheckBox = new QCheckBox, 0, 1);

    QGroupBox* unitsBox = new QGroupBox(tr("Units"));
    QGridLayout* unitsLayout = new QGridLayout;
    unitsBox->setLayout(unitsLayout);

    unitsLayout->addWidget(new QLabel(tr("Length")), 0, 0);
    unitsLayout->addWidget(m_cb_LengthUnits = new LengthUnitsComboBox, 0, 1);
    unitsLayout->addWidget(new QLabel(tr("Angle")), 1, 0);
    unitsLayout->addWidget(m_cb_AngleUnits = new AngleUnitsComboBox, 1, 1);
    unitsLayout->addWidget(new QLabel(tr("Pressure")), 2, 0);
    unitsLayout->addWidget(m_cb_PressureUnits = new PressureUnitsComboBox, 2, 1);
    unitsLayout->addWidget(new QLabel(tr("Temperature")), 3, 0);
    unitsLayout->addWidget(m_cb_TemperatureUnits = new TemperatureUnitsComboBox, 3, 1);
    unitsLayout->addWidget(new QLabel(tr("Force")), 4, 0);
    unitsLayout->addWidget(m_cb_ForceUnits = new ForceUnitsComboBox, 4, 1);
    unitsLayout->addWidget(new QLabel(tr("Mass")), 5, 0);
    unitsLayout->addWidget(m_cb_MassUnits = new MassUnitsComboBox, 5, 1);
    unitsLayout->addWidget(m_defaultUnitsButton = new QPushButton(tr("Default")), 255, 0);
    unitsLayout->addWidget(m_metricUnitsButton = new QPushButton(tr("Metric")), 255, 1);
    unitsLayout->addWidget(m_imperialUnitsButton = new QPushButton(tr("Imperial")), 255, 2);

    m_applyButton = new QPushButton(tr("Apply"), this);
    m_cancelButton = new QPushButton(tr("Close"), this);

    
    QGridLayout* masterLayout = new QGridLayout;
    masterLayout->addWidget(settingsBox, 0, 0, 1, 2);
    masterLayout->addWidget(unitsBox, 1, 0, 1, 2);
    masterLayout->addWidget(m_applyButton, 255, 0);        
    masterLayout->addWidget(m_cancelButton, 255, 1);

    setLayout(masterLayout);
}
void GlobalSettingsDialog::ApplySettings()
{
    if (m_GlobalSettings)
    {
        m_GlobalSettings->m_LengthUnits = m_cb_LengthUnits->GetCurrentUnits();
        m_GlobalSettings->m_AngleUnits = m_cb_AngleUnits->GetCurrentUnits();
        m_GlobalSettings->m_PressureUnits = m_cb_PressureUnits->GetCurrentUnits();
        m_GlobalSettings->m_TemperatureUnits = m_cb_TemperatureUnits->GetCurrentUnits();
        m_GlobalSettings->m_ForceUnits = m_cb_ForceUnits->GetCurrentUnits();
        m_GlobalSettings->m_MassUnits = m_cb_MassUnits->GetCurrentUnits();
        m_GlobalSettings->m_redrawOnChanges = m_reSimCheckBox->isChecked();
        emit m_GlobalSettings->SettingsChanged();
    }
}
void GlobalSettingsDialog::PopulateSettings(SETTINGS_POPULATE_TYPE type)
{
    if (m_GlobalSettings)
    {
        m_reSimCheckBox->setChecked(m_GlobalSettings->m_redrawOnChanges);
        if (type == SETTINGS_POPULATE_TYPE::current_)
        {
            m_cb_LengthUnits->setCurrentIndex(int(m_GlobalSettings->m_LengthUnits));
            m_cb_AngleUnits->setCurrentIndex(int(m_GlobalSettings->m_AngleUnits));
            m_cb_PressureUnits->setCurrentIndex(int(m_GlobalSettings->m_PressureUnits));
            m_cb_TemperatureUnits->setCurrentIndex(int(m_GlobalSettings->m_TemperatureUnits));
            m_cb_ForceUnits->setCurrentIndex(int(m_GlobalSettings->m_ForceUnits));
            m_cb_MassUnits->setCurrentIndex(int(m_GlobalSettings->m_MassUnits));
        }
        else if (type == SETTINGS_POPULATE_TYPE::default_)
        {
            m_cb_LengthUnits->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::inches));
            m_cb_AngleUnits->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_cb_PressureUnits->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::psi));
            m_cb_TemperatureUnits->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::fahrenheit));
            m_cb_ForceUnits->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::newtons));
            m_cb_MassUnits->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::pounds_mass));
        }
        else if (type == SETTINGS_POPULATE_TYPE::metric_)
        {
            m_cb_LengthUnits->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::millimeters));
            m_cb_AngleUnits->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_cb_PressureUnits->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::kilopascals));
            m_cb_TemperatureUnits->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::kelvin));
            m_cb_ForceUnits->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::newtons));
            m_cb_MassUnits->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::kilograms));
        }
        else if (type == SETTINGS_POPULATE_TYPE::imperial_)
        {
            m_cb_LengthUnits->setCurrentIndex(int(OpenBurnUnits::LengthUnits_T::inches));
            m_cb_AngleUnits->setCurrentIndex(int(OpenBurnUnits::AngleUnits_T::degrees));
            m_cb_PressureUnits->setCurrentIndex(int(OpenBurnUnits::PressureUnits_T::psi));
            m_cb_TemperatureUnits->setCurrentIndex(int(OpenBurnUnits::TemperatureUnits_T::fahrenheit));
            m_cb_ForceUnits->setCurrentIndex(int(OpenBurnUnits::ForceUnits_T::pounds_force));
            m_cb_MassUnits->setCurrentIndex(int(OpenBurnUnits::MassUnits_T::pounds_mass));
        }
    }
}
void GlobalSettingsDialog::ApplyButton_Clicked()
{
    ApplySettings();
}
void GlobalSettingsDialog::CancelButton_Clicked()
{
    close();
}
void GlobalSettingsDialog::DefaultUnitsButton_Clicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::default_);
}
void GlobalSettingsDialog::MetricUnitsButton_Clicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::metric_);
}
void GlobalSettingsDialog::ImperialUnitsButton_Clicked()
{
    PopulateSettings(SETTINGS_POPULATE_TYPE::imperial_);
}
