#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

#include "src/ui/widgets/unitscombobox.h"
#include "src/export.h"
class EngExportDialog : public QDialog
{
public:
    explicit EngExportDialog(
        const EngExport& exporter,
        MotorSim* simulator,
        OpenBurnSettings* settings,
        QWidget* parent = nullptr);
private slots:
    void OnOKButtonClicked();
    void OnCancelButtonClicked();
private:
    void SetupUI();
    void SetParams();
    LengthUnitsComboBox *m_UnitsMotorLen, *m_UnitsMotorDia;
    QDoubleSpinBox *m_sbMotorLen, *m_sbMotorDia;

    MassUnitsComboBox *m_UnitsCaseMass;
    QDoubleSpinBox *m_sbCaseMass;

    QLineEdit *m_lneMotorMfg, *m_lneMotorName;
    QPushButton *m_btnOk, *m_btnCancel;
    EngExport m_Exporter;
    MotorSim* m_Simulator;
};
