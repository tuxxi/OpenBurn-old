#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

#include "src/ui/dialogs/graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, OpenBurnGrain* seedValues, QList<OpenBurnGrain*>grains) :
    QDialog(parent), m_gfxGrain(nullptr), m_GrainsToEdit(grains), m_isNewGrainWindow(m_GrainsToEdit.isEmpty())
{
    SetupGraphics();
    SetupUI(seedValues); //setup the ui and populate the various options with the "seed" values
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));
    connect(m_GrainDesign, SIGNAL(SIG_GrainType_Changed(GRAINTYPE)), this, SLOT(RefreshUI(GRAINTYPE)));
    connect(m_GrainDesign, SIGNAL(SIG_GrainDesign_Changed()), this, SLOT(UpdateDesign()));
    setAttribute(Qt::WA_DeleteOnClose);
}
GrainDialog::~GrainDialog()
{

}
void GrainDialog::SetupUI(OpenBurnGrain* seed)
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("GrainDialog"));        
    }

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    m_controlsLayout = new QGridLayout;
    m_frame = new QGroupBox(tr("Grain Design"));
    m_frame->setLayout(m_controlsLayout); 

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(1);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);    
    BatesGrain* BatesSeed = dynamic_cast<BatesGrain*>(seed);

    //Default to BATES grain design mode
    m_GrainDesign = new BatesGrainDesign(m_frame, BatesSeed);        

    // OK and Cancel buttons
    m_applyButton = new QPushButton(m_frame);
    m_cancelButton = new QPushButton(m_frame);
    m_applyButton->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_cancelButton->setText(tr("Close"));

    m_controlsLayout->addWidget(m_GrainDesign, 1, 0, 3, 3);
    //we use a value of 256 here to ensure that we can have a LOT of extra info padded in there.
    m_controlsLayout->addWidget(m_applyButton, 256, 0);
    m_controlsLayout->addWidget(m_cancelButton, 256, 1);

    QVBoxLayout* masterVLayout = new QVBoxLayout;
    masterVLayout->addWidget(m_frame);    
    masterVLayout->addWidget(m_graphicsView);
    setLayout(masterVLayout);
    UpdateDesign();
}
void GrainDialog::SetupGraphics()
{
    m_graphicsView = new QGraphicsView;
    m_graphicsScene = new QGraphicsScene;
    m_graphicsView->setScene(m_graphicsScene);
    m_graphicsView->show();
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(true);
    m_graphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::RefreshUI(GRAINTYPE type)
{
    m_controlsLayout->removeWidget(m_GrainDesign);
    m_GrainDesign->deleteLater();
    if (type == GRAINTYPE_BATES)
    {
        m_GrainDesign = new BatesGrainDesign(m_frame, static_cast<BatesGrain*>(m_GrainsToEdit.first()));
        m_controlsLayout->addWidget(m_GrainDesign, 1, 0, 3, 3);
        connect(m_GrainDesign, SIGNAL(SIG_GrainType_Changed(GRAINTYPE)), this, SLOT(RefreshUI(GRAINTYPE))); 
        connect(m_GrainDesign, SIGNAL(SIG_GrainDesign_Changed()), this, SLOT(UpdateDesign()));        
    }
    else
    {
        //other grain types
    }
    UpdateDesign();    
}
void GrainDialog::UpdateDesign()
{
    BatesGrainDesign* design = dynamic_cast<BatesGrainDesign*>(m_GrainDesign);
    if (design)
    {
        if (m_isNewGrainWindow && m_GrainsToEdit.isEmpty())
        {
            OpenBurnPropellant* prop = new OpenBurnPropellant();
            double rho = OpenBurnUtil::CONVERT_PoundsToSlugs(0.065f);
            prop->SetBasicParams(0.0665f, 0.319f, 3100.f, rho);
            prop->SetPropellantName("KNSU - TEST");
            OpenBurnGrain* grain = new BatesGrain(
                design->GetDiameter(),        
                design->GetCoreDiameter(),
                design->GetLength(),
                prop,                    
                design->GetInhibitedFaces());
            m_GrainsToEdit.push_back(grain);
        }
        else
        {
            for (auto* i : m_GrainsToEdit)
            {
                BatesGrain* grain = static_cast<BatesGrain*>(i);
                grain->SetDiameter(design->GetDiameter());
                grain->SetCoreDiameter(design->GetCoreDiameter());
                grain->SetLength(design->GetLength());
                grain->SetInhibitedFaces(design->GetInhibitedFaces());
                i = grain;
            }
        }
    }
    UpdateGraphics();
}
void GrainDialog::UpdateGraphics()
{
    if (!m_gfxGrain)
    {
        m_gfxGrain = new GrainGraphicsItem(m_GrainsToEdit.front(), 100, false);
        m_graphicsScene->addItem(m_gfxGrain);
    }
    m_gfxGrain->setPos(0, 0);    
    m_gfxGrain->update(m_gfxGrain->boundingRect());
    m_graphicsView->viewport()->repaint();

    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_graphicsView->setSceneRect(m_gfxGrain->boundingRect());
    QRectF bounds = QRectF(m_gfxGrain->boundingRect().left(), m_gfxGrain->boundingRect().top(), 
        m_gfxGrain->boundingRect().width() + 50, m_gfxGrain->boundingRect().height() + 50);
    m_graphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_graphicsView->viewport()->repaint();
    m_gfxGrain->update(m_gfxGrain->boundingRect());    
}
void GrainDialog::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
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
    UpdateDesign();
    if (m_isNewGrainWindow)
    {
        emit SIG_DIALOG_NewGrain(m_GrainsToEdit.front());
        m_GrainsToEdit.pop_front();        
    }
    else
    {
        for (auto* i : m_GrainsToEdit)
        {
            emit SIG_DIALOG_NewGrain(i);            
        }    
    }
}
