#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <src/application.hpp>

#include "src/motorsim.hpp"
#include "src/ui/widgets/unitscombobox.hpp"
class SimSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimSettingsDialog(
        OpenBurnApplication& app,
        QWidget* parent = nullptr);
    ~SimSettingsDialog() = default;

private slots:
    void OnApplyButtonClicked();
    void OnCancelButtonClicked();
private:
    void SetupUI();
    void ApplySettings();
    OpenBurnApplication& m_app;
    QPushButton* m_btnApply, *m_btnCancel;
    QDoubleSpinBox* m_sbAmbientPressure, *m_sbAmbientTemp, *m_sbTwoPhaseFlow, *m_sbSkinFriction, *m_sbTimeStep;
    PressureUnitsComboBox* m_unitsAmbientPressure;
    TemperatureUnitsComboBox* m_unitsAmbientTemp;
};
