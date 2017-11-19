#pragma once
#include <QDialog>
#include <QPushButton>
#include <QCheckBox>

#include "src/settings.h"
#include "src/ui/widgets/unitscombobox.h"

enum class SETTINGS_POPULATE_TYPE
{
    current,
    def, //can't use the literal "default" lmao
    metric,
    imperial
};
class GlobalSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GlobalSettingsDialog(OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~GlobalSettingsDialog() = default;
private slots:
    void OnApplyButtonClicked();
    void OnCancelButtonClicked();

    void OnDefaultUnitsButtonClicked();
    void OnMetricUnitsButtonClicked();
    void OnImperialUnitsButtonClicked();

private:
    void SetupUI();
    void ApplySettings();
    void PopulateSettings(SETTINGS_POPULATE_TYPE type);
    OpenBurnSettings* m_GlobalSettings;

    QPushButton* m_btnApply, *m_btnCancel;

    QCheckBox* m_chbxReSimOnChanges;

    //units
    QPushButton* m_btnDefaultUnits, *m_btnImperialUnits, *m_btnMetricUnits;
    LengthUnitsComboBox* m_unitsLength;
    AngleUnitsComboBox* m_unitsAngle;
    TemperatureUnitsComboBox* m_unitsTemperature;
    PressureUnitsComboBox* m_unitsPressure;
    ForceUnitsComboBox* m_unitsForce;
    MassUnitsComboBox* m_unitsMass;
};
