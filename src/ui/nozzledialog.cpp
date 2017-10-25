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

    m_OKButton = new QPushButton(tr("OK"), this);
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_applyButton = new QPushButton(tr("Apply"), this);

    //throat diameter
    m_throatDiaSpinBox = new QDoubleSpinBox(this);
    m_throatDiaSpinBox->setDecimals(3);
    m_throatDiaSpinBox->setSingleStep(0.25f);
    QLabel* label = new QLabel(tr("Throat Diameter"), this);
    label->setBuddy(m_throatDiaSpinBox);
    m_throatDiaUnits = new QComboBox(this);
    m_throatDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label, 0, 0);
    layout->addWidget(m_throatDiaSpinBox, 0, 1);
    layout->addWidget(m_throatDiaUnits, 0, 2);

    //exit (expansion, divergence) diameter
    m_exitDiaSpinBox = new QDoubleSpinBox(this);
    m_exitDiaSpinBox->setDecimals(3);
    m_exitDiaSpinBox->setSingleStep(0.25f);
    QLabel* label_2 = new QLabel(tr("Exit Diameter"), this);
    label_2->setBuddy(m_exitDiaSpinBox);
    m_exitDiaUnits = new QComboBox(this);
    m_exitDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label_2, 1, 0);
    layout->addWidget(m_exitDiaSpinBox, 1, 1);
    layout->addWidget(m_exitDiaUnits, 1, 2);
    
    setTabOrder(m_throatDiaSpinBox, m_exitDiaSpinBox);
    setTabOrder(m_exitDiaSpinBox, m_OKButton);
    setTabOrder(m_OKButton, m_cancelButton);

    //buttons
    layout->addWidget(m_OKButton, 2, 0);
    layout->addWidget(m_applyButton, 2, 1);        
    layout->addWidget(m_cancelButton, 2, 2);
    
    setLayout(layout);
}
bool NozzleDialog::apply()
{
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_throatDiaSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Throat Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    if (qFuzzyIsNull(m_exitDiaSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Exit Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    ConicalNozzle* nozz = new ConicalNozzle(m_throatDiaSpinBox->value(), m_exitDiaSpinBox->value());
    emit SIG_NozzleChanged(nozz);
    return true;
}
void NozzleDialog::accept()
{
    if (apply())
    {
        closeDialog();        
    }
}
void NozzleDialog::closeDialog()
{
    close();
}
