#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>

#include "src/ui/dialogs/nozzledialog.h"
NozzleDialog::NozzleDialog(QWidget* parent, OpenBurnNozzle* seed, OpenBurnSettings* settings)
    : QDialog(parent), m_Nozzle(seed), m_gfxNozzle(nullptr), m_GlobalSettings(settings)
{
    SetupUI();
    connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(closeDialog()));        
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(apply()));  
    
    connect(m_NozzleDesign, SIGNAL(SIG_DesignUpdated()), this, SLOT(UpdateDesign()));
    setAttribute(Qt::WA_DeleteOnClose);
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

    ConicalNozzle* nozz = dynamic_cast<ConicalNozzle*>(m_Nozzle);
    m_NozzleDesign = new ConicalNozzleDesign(this, nozz, m_GlobalSettings);
    m_OKButton = new QPushButton(tr("OK"), this);
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_applyButton = new QPushButton(tr("Apply"), this);

    //buttons
    layout->addWidget(m_NozzleDesign, 0, 0, 1, 3);
    layout->addWidget(m_OKButton, 255, 0);
    layout->addWidget(m_applyButton, 255, 1);        
    layout->addWidget(m_cancelButton, 255, 2);
    
    m_graphicsView = new QGraphicsView;
    m_graphicsScene = new QGraphicsScene;
    m_graphicsView->setScene(m_graphicsScene);
    m_graphicsView->show();
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(10);
    sizePolicy2.setHeightForWidth(m_graphicsView->sizePolicy().hasHeightForWidth());
    m_graphicsView->setSizePolicy(sizePolicy2);

    QVBoxLayout* masterLayout = new QVBoxLayout(this);
    
    masterLayout->addWidget(m_frame);
    masterLayout->addWidget(m_graphicsView);
    setLayout(masterLayout);
}
void NozzleDialog::UpdateDesign()
{
    ConicalNozzleDesign* design = dynamic_cast<ConicalNozzleDesign*>(m_NozzleDesign);
    if (design)
    {
        if (!m_Nozzle)
        {
            m_Nozzle = new ConicalNozzle(
                design->GetThroatDiameter(), 
                design->GetExitDiameter(), 
                design->GetDivergentHalfAngle());    
        }
        else
        {
            ConicalNozzle* nozz = static_cast<ConicalNozzle*>(m_Nozzle);
            nozz->SetNozzleThroat(design->GetThroatDiameter());
            nozz->SetNozzleExit(design->GetExitDiameter());
            nozz->SetHalfAngle(design->GetDivergentHalfAngle());
        }
    }
    UpdateGraphics();
}
void NozzleDialog::UpdateGraphics()
{
    if (!m_gfxNozzle)
    {
        m_gfxNozzle = new NozzleGraphicsItem(m_Nozzle, 100, 100, false);
        m_graphicsScene->addItem(m_gfxNozzle);    
    }
    QRectF bounding = m_gfxNozzle->boundingRect();
    m_gfxNozzle->setPos(0, 0);    
    m_gfxNozzle->update(bounding);
    m_graphicsView->viewport()->repaint();

    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_graphicsView->setSceneRect(bounding);
    QRectF bounds = QRectF(bounding.left(), bounding.top(), 
        bounding.width() + 50, bounding.height() + 50);
    m_graphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_graphicsView->viewport()->repaint();    
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
    UpdateDesign();    
    emit SIG_NozzleChanged(m_Nozzle);
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
