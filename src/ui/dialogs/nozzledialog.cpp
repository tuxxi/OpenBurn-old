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
    connect(m_btnOK, &QPushButton::clicked,
            this, &NozzleDialog::accept);
    connect(m_btnClose, &QPushButton::clicked,
            this, &NozzleDialog::OnCloseButtonClicked);
    connect(m_btnApply, &QPushButton::clicked,
            this, &NozzleDialog::OnApplyButtonClicked);
    
    connect(m_NozzleDesign, &OpenBurnDesignNozzle::DesignUpdated,
            this, &NozzleDialog::OnDesignUpdated);
    setAttribute(Qt::WA_DeleteOnClose);
}
void NozzleDialog::SetupUI()
{
    QGridLayout* layout = new QGridLayout(this);
    setWindowTitle(tr("Modify Nozzle"));

    m_Frame = new QFrame(this);
    m_Frame->setLayout(layout); 
    m_Frame->setFrameShape(QFrame::StyledPanel);
    m_Frame->setFrameShadow(QFrame::Raised);

    ConicalNozzle* nozz = dynamic_cast<ConicalNozzle*>(m_Nozzle);
    m_NozzleDesign = new ConicalNozzleDesign(this, nozz, m_GlobalSettings);
    m_btnOK = new QPushButton(tr("OK"), this);
    m_btnClose = new QPushButton(tr("Cancel"), this);
    m_btnApply = new QPushButton(tr("Apply"), this);

    //buttons
    layout->addWidget(m_NozzleDesign, 0, 0, 1, 3);
    layout->addWidget(m_btnOK, 255, 0);
    layout->addWidget(m_btnApply, 255, 1);        
    layout->addWidget(m_btnClose, 255, 2);
    
    m_NozzleGraphicsView = new QGraphicsView;
    m_NozzleGraphicsScene = new QGraphicsScene;
    m_NozzleGraphicsView->setScene(m_NozzleGraphicsScene);
    m_NozzleGraphicsView->show();
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(10);
    sizePolicy2.setHeightForWidth(m_NozzleGraphicsView->sizePolicy().hasHeightForWidth());
    m_NozzleGraphicsView->setSizePolicy(sizePolicy2);

    QVBoxLayout* masterLayout = new QVBoxLayout(this);
    
    masterLayout->addWidget(m_Frame);
    masterLayout->addWidget(m_NozzleGraphicsView);
    setLayout(masterLayout);
}
void NozzleDialog::OnDesignUpdated()
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
        m_NozzleGraphicsScene->addItem(m_gfxNozzle);    
    }
    QRectF bounding = m_gfxNozzle->boundingRect();
    m_gfxNozzle->setPos(0, 0);    
    m_gfxNozzle->update(bounding);
    m_NozzleGraphicsView->viewport()->repaint();

    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_NozzleGraphicsView->setSceneRect(bounding);
    QRectF bounds = QRectF(bounding.left(), bounding.top(), 
        bounding.width() + 50, bounding.height() + 50);
    m_NozzleGraphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_NozzleGraphicsView->viewport()->repaint();    
}
bool NozzleDialog::OnApplyButtonClicked()
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
    OnDesignUpdated();    
    emit NozzleChanged(m_Nozzle);
    return true;    
}
void NozzleDialog::accept()
{
    if (OnApplyButtonClicked())
    {
        OnCloseButtonClicked();        
    }
}
void NozzleDialog::OnCloseButtonClicked()
{
    close();
}
