#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>

#include "src/ui/dialogs/nozzledialog.h"

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
    QGridLayout* layout = new QGridLayout(this);
    setWindowTitle(tr("Modify Nozzle"));

    m_frame = new QFrame(this);
    m_frame->setLayout(layout); 
    m_frame->setFrameShape(QFrame::StyledPanel);
    m_frame->setFrameShadow(QFrame::Raised);

    m_NozzleDesign = new ConicalNozzleDesign(this);
    m_OKButton = new QPushButton(tr("OK"), this);
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_applyButton = new QPushButton(tr("Apply"), this);

    //buttons
    layout->addWidget(m_NozzleDesign, 0, 0, 1, 3);
    layout->addWidget(m_OKButton, 255, 0);
    layout->addWidget(m_applyButton, 255, 1);        
    layout->addWidget(m_cancelButton, 255, 2);
    
    m_graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_graphicsView->sizePolicy().hasHeightForWidth());
    m_graphicsView->setSizePolicy(sizePolicy2);

    QVBoxLayout* masterLayout = new QVBoxLayout(this);
    
    masterLayout->addWidget(m_frame);
    masterLayout->addWidget(m_graphicsView);
    setLayout(masterLayout);
}
bool NozzleDialog::apply()
{
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_NozzleDesign->GetThroatDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Throat Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    if (qFuzzyIsNull(m_NozzleDesign->GetExitDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Exit Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    ConicalNozzleDesign* design = dynamic_cast<ConicalNozzleDesign*>(m_NozzleDesign);
    if (design)
    {
        ConicalNozzle* nozz = new ConicalNozzle(
            design->GetThroatDiameter(), 
            design->GetExitDiameter(), 
            design->GetDivergentHalfAngle());
        emit SIG_NozzleChanged(nozz);
        return true;    
    }
    return false;
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
