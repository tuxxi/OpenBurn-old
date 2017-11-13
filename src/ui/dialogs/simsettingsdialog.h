#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>

#include "src/motorsim.h"
class SimSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimSettingsDialog(MotorSimSettings* settings, QWidget* parent = nullptr);
    ~SimSettingsDialog();
private slots:
    void ApplyButton_Clicked();
    void CancelButton_Clicked();
private:
    void SetupUI();
    void ApplySettings();
    QPushButton* m_applyButton, *m_cancelButton;
    QDoubleSpinBox* m_sb_ambientPressure, *m_sb_twoPhaseFlow, *m_sb_skinFriction, *m_sb_timeStep;
    QComboBox* m_cb_ambientPressureUnits;
    MotorSimSettings* m_Settings;
};
