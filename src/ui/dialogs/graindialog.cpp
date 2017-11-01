#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

#include "src/ui/dialogs/graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, OpenBurnGrain* grain) :
    QDialog(parent), m_isNewGrainWindow(grain == nullptr), m_Grain(grain)
{
    SetupUI();
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));
    connect(m_GrainDesign, SIGNAL(SIG_GrainType_Changed(GRAINTYPE)), this, SLOT(SLOT_GrainType_Changed(GRAINTYPE)));
    setAttribute(Qt::WA_DeleteOnClose);
}
GrainDialog::~GrainDialog()
{
}
void GrainDialog::SetupUI()
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("GrainDialog"));        
    }

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    m_controlsLayout = new QGridLayout;
    m_frame = new QFrame(this);
    m_frame->setLayout(m_controlsLayout); 
    m_frame->setFrameShape(QFrame::StyledPanel);
    m_frame->setFrameShadow(QFrame::Raised);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_frame->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);    
    m_GrainDesign = new BatesGrainDesign(m_frame, static_cast<BatesGrain*>(m_Grain));

    // OK and Cancel buttons
    m_applyButton = new QPushButton(m_frame);
    m_cancelButton = new QPushButton(m_frame);
    m_applyButton->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_cancelButton->setText(tr("Close"));

    m_controlsLayout->addWidget(m_GrainDesign, 1, 0, 3, 3);
    //we use a value of 256 here to ensure that we can have a LOT of extra info padded in there.
    m_controlsLayout->addWidget(m_applyButton, 256, 0);
    m_controlsLayout->addWidget(m_cancelButton, 256, 1);

    m_graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_graphicsView->sizePolicy().hasHeightForWidth());
    m_graphicsView->setSizePolicy(sizePolicy2);

    QVBoxLayout* masterVLayout = new QVBoxLayout;
    masterVLayout->addWidget(m_frame);    
    masterVLayout->addWidget(m_graphicsView);

    setLayout(masterVLayout);
}
void GrainDialog::RefreshUI()
{
    m_controlsLayout->removeWidget(m_GrainDesign);
    m_GrainDesign->deleteLater();
    if (m_GrainType == GRAINTYPE_BATES)
    {
        m_GrainDesign = new BatesGrainDesign(m_frame, static_cast<BatesGrain*>(m_Grain));
        m_controlsLayout->addWidget(m_GrainDesign, 1, 0, 3, 3);
        connect(m_GrainDesign, SIGNAL(SIG_GrainType_Changed(GRAINTYPE)), this, SLOT(SLOT_GrainType_Changed(GRAINTYPE)));   
    }
    else
    {
        //other grain types
    }
}
void GrainDialog::SLOT_GrainType_Changed(GRAINTYPE type)
{
    m_GrainType = type;
    RefreshUI();
}
void GrainDialog::on_cancelButton_clicked()
{
    close();
}
void GrainDialog::on_applyButton_clicked()
{
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_GrainDesign->GetLength()) )
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Length cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_GrainDesign->GetDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (m_GrainType == GRAINTYPE_BATES)
    {
        BatesGrainDesign* design = static_cast<BatesGrainDesign*>(m_GrainDesign);
        if (m_isNewGrainWindow)
        {
            OpenBurnPropellant* prop = new OpenBurnPropellant;            
            BatesGrain *grain = new BatesGrain(
                design->GetDiameter(),        
                design->GetCoreDiameter(),
                design->GetLength(),
                prop,                    
                design->GetInhibitedFaces());
            emit SIG_DIALOG_NewGrain(grain);
    
        }
        else //Editing grain
        {
            //edit grain
            BatesGrain* grain = static_cast<BatesGrain*>(m_Grain);
            grain->SetDiameter(design->GetDiameter());
            grain->SetCoreDiameter(design->GetCoreDiameter());
            grain->SetLength(design->GetLength());
            grain->SetInhibitedFaces(design->GetInhibitedFaces());
            //m_Grain->SetPropellantType();
            emit SIG_DIALOG_EditGrain(m_Grain);
        }        

    }
}
