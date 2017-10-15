#include "src/ui/nozzledialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>

NozzleDialog::NozzleDialog(QWidget* parent)
    : QDialog(parent)
{
    SetupUI();
    connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(closeDialog()));        
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(apply()));    
}
NozzleDialog::~NozzleDialog()
{

}
void NozzleDialog::SetupUI()
{
    QGridLayout *layout = new QGridLayout;
    setWindowTitle(tr("Modify Nozzle"));

    m_OKButton = new QPushButton(tr("OK"));
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_applyButton = new QPushButton(tr("Apply"));

    //throat diameter
    m_throatDiaSpinBox = new QDoubleSpinBox;
    m_throatDiaSpinBox->setDecimals(3);
    m_throatDiaSpinBox->setSingleStep(0.25f);
    QLabel* label = new QLabel(tr("Throat Diameter"));
    label->setBuddy(m_throatDiaSpinBox);
    m_throatDiaUnits = new QComboBox;
    m_throatDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label, 0, 0);
    layout->addWidget(m_throatDiaSpinBox, 0, 1);
    layout->addWidget(m_throatDiaUnits, 0, 2);

    //exit (expansion, divergence) diameter
    m_exitDiaSpinBox = new QDoubleSpinBox;
    m_exitDiaSpinBox->setDecimals(3);
    m_exitDiaSpinBox->setSingleStep(0.25f);
    QLabel* label_2 = new QLabel(tr("Exit Diameter"));
    label_2->setBuddy(m_exitDiaSpinBox);
    m_exitDiaUnits = new QComboBox;
    m_exitDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label_2, 1, 0);
    layout->addWidget(m_exitDiaSpinBox, 1, 1);
    layout->addWidget(m_exitDiaUnits, 1, 2);

    //buttons
    layout->addWidget(m_OKButton, 2, 0);
    layout->addWidget(m_applyButton, 2, 1);        
    layout->addWidget(m_cancelButton, 2, 2);
    
    setLayout(layout);
}
void NozzleDialog::apply()
{
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_throatDiaSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Throat Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_exitDiaSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Exit Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ConicalNozzle* nozz = new ConicalNozzle(m_throatDiaSpinBox->value(), m_exitDiaSpinBox->value());
    emit SIG_NozzleChanged(nozz);
}
void NozzleDialog::accept()
{
    apply();
    closeDialog();
}
void NozzleDialog::closeDialog()
{
    close();
}
