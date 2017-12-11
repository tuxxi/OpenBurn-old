#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>

#include "src/motorsim.h"
#include "src/ui/widgets/unitscombobox.h"
class SimSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimSettingsDialog(
        MotorSimSettings* settings,
        OpenBurnSettings* globalSettings,
        QWidget* parent = nullptr);
    ~SimSettingsDialog() = default;
    void SetGlobalSettings(OpenBurnSettings* settings);

private slots:
    void OnApplyButtonClicked();
    void OnCancelButtonClicked();
private:
    void SetupUI();
    void ApplySettings();
    QPushButton* m_btnApply, *m_btnCancel;
    QDoubleSpinBox* m_sbAmbientPressure, *m_sbAmbientTemp, *m_sbTwoPhaseFlow, *m_sbSkinFriction, *m_sbTimeStep;
    PressureUnitsComboBox* m_unitsAmbientPressure;
    TemperatureUnitsComboBox* m_unitsAmbientTemp;
    MotorSimSettings* m_Settings;
};
