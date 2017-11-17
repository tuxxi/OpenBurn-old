#pragma once
#include <QDialog>
#include <QPushButton>
#include <QCheckBox>

#include "src/settings.h"
#include "src/ui/widgets/unitscombobox.h"

enum class SETTINGS_POPULATE_TYPE
{
    current_,
    default_,
    metric_,
    imperial_
};
class GlobalSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GlobalSettingsDialog(OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~GlobalSettingsDialog();
private slots:
    void ApplyButton_Clicked();
    void CancelButton_Clicked();

    void DefaultUnitsButton_Clicked();
    void MetricUnitsButton_Clicked();
    void ImperialUnitsButton_Clicked();

private:
    void SetupUI();
    void ApplySettings();
    void PopulateSettings(SETTINGS_POPULATE_TYPE type);
    OpenBurnSettings* m_GlobalSettings;

    QPushButton* m_applyButton, *m_cancelButton;
    QPushButton* m_defaultUnitsButton, *m_imperialUnitsButton, *m_metricUnitsButton;
    QCheckBox* m_reSimCheckBox;
    LengthUnitsComboBox* m_cb_LengthUnits;
    AngleUnitsComboBox* m_cb_AngleUnits;
    TemperatureUnitsComboBox* m_cb_TemperatureUnits;
    PressureUnitsComboBox* m_cb_PressureUnits;
    ForceUnitsComboBox* m_cb_ForceUnits;
    MassUnitsComboBox* m_cb_MassUnits;
};
