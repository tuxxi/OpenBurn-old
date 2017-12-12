#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include "exportdialog.hpp"
#include <QFileDialog>

EngExportDialog::EngExportDialog(const EngExport &exporter,
                                 MotorSim *simulator,
                                 OpenBurnSettings *settings,
                                 QWidget *parent)
    : QDialog(parent),
    m_Exporter(exporter),
    m_Simulator(simulator)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    connect(m_btnOk, &QPushButton::clicked,
        this, &EngExportDialog::OnOKButtonClicked);
    connect(m_btnCancel, &QPushButton::clicked,
        this, &EngExportDialog::OnCancelButtonClicked);
    if (m_Simulator)
    {
        m_sbMotorLen->setValue(m_Simulator->GetDesignMotor()->GetMotorLength());
        m_sbMotorDia->setValue(m_Simulator->GetDesignMotor()->GetMotorMajorDiameter());
        m_lneMotorName->setText(m_Simulator->GetMotorDesignation());
    }
    m_UnitsCaseMass->SetUnits(settings->m_MassUnits);
    m_UnitsMotorDia->SetUnits(settings->m_LengthUnits);
    m_UnitsMotorLen->SetUnits(settings->m_LengthUnits);

}
void EngExportDialog::SetupUI()
{
    setWindowTitle(tr("Export to .eng"));
    auto* controlsLayout = new QGridLayout;
    auto* box = new QGroupBox(tr(".eng File Settings"));

    controlsLayout->addWidget(new QLabel(tr("Motor Code:")), 0, 0);
    controlsLayout->addWidget(m_lneMotorName = new QLineEdit, 0, 1);
    controlsLayout->addWidget(new QLabel(tr("Manufacturer:")), 1, 0);
    controlsLayout->addWidget(m_lneMotorMfg = new QLineEdit, 1, 1);
    controlsLayout->addWidget(new QLabel(tr("Hardware (case) mass:")), 2, 0);
    controlsLayout->addWidget(m_sbCaseMass = new QDoubleSpinBox, 2, 1);
    controlsLayout->addWidget(m_UnitsCaseMass = new MassUnitsComboBox(this, m_sbCaseMass), 2, 2);
    controlsLayout->addWidget(new QLabel(tr("Motor Diameter:")), 3, 0);
    controlsLayout->addWidget(m_sbMotorDia = new QDoubleSpinBox, 3, 1);
    controlsLayout->addWidget(m_UnitsMotorDia = new LengthUnitsComboBox(this, m_sbMotorDia), 3, 2);
    controlsLayout->addWidget(new QLabel(tr("Motor Length:")), 4, 0);
    controlsLayout->addWidget(m_sbMotorLen = new QDoubleSpinBox, 4, 1);
    controlsLayout->addWidget(m_UnitsMotorLen = new LengthUnitsComboBox(this, m_sbMotorLen), 4, 2);
    box->setLayout(controlsLayout);

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_btnOk = new QPushButton(tr("Ok")));
    buttonsLayout->addWidget(m_btnCancel = new QPushButton(tr("Cancel")));

    auto* masterLayout = new QVBoxLayout;
    masterLayout->addWidget(box);
    masterLayout->addLayout(buttonsLayout);
    setLayout(masterLayout);
}

void EngExportDialog::SetParams()
{
    const double caseMass = m_UnitsCaseMass->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::MassUnits_T::kilograms, 
        m_sbCaseMass->value());
    const int motorLen = static_cast<const int>(m_UnitsMotorLen->GetCurrentUnits().ConvertTo(
            OpenBurnUnits::LengthUnits_T::millimeters,
            m_sbMotorLen->value()));
    const int motorDia = static_cast<const int>(m_UnitsMotorDia->GetCurrentUnits().ConvertTo(
            OpenBurnUnits::LengthUnits_T::millimeters,
            m_sbMotorDia->value()));

    const auto mfgName = m_lneMotorMfg->text();
    const auto motorName = m_lneMotorName->text();

    m_Exporter.SetMotorDetails(caseMass, motorLen, motorDia, motorName, mfgName);
}

void EngExportDialog::OnOKButtonClicked()
{
    SetParams();
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
        tr("eng file (*.eng)"));
    m_Exporter.WriteToEngFile(fileName);
    close();
}

void EngExportDialog::OnCancelButtonClicked()
{
    close();
}
