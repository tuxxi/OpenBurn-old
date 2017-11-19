#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

#include "src/ui/dialogs/graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(PropellantList* prop,
    OpenBurnGrain* seedValues,
    OpenBurnSettings *settings,
    QList<OpenBurnGrain*>grains,
    QWidget *parent)
    : QDialog(parent), m_gfxGrain(nullptr), m_GrainsToEdit(grains),
    m_Propellants(prop),
    m_GlobalSettings(settings),
    m_isNewGrainWindow(m_GrainsToEdit.isEmpty())
{
    SetupGraphics();
    SetupUI(seedValues); //setup the ui and populate the various options with the "seed" values
    connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
    connect(m_btnApply, SIGNAL(clicked()), this, SLOT(OnApplyButtonClicked()));
    connect(m_GrainDesign, SIGNAL(GrainDesignChanged()), this, SLOT(OnDesignUpdated()));
    setAttribute(Qt::WA_DeleteOnClose);
}
void GrainDialog::SetupUI(OpenBurnGrain* seed)
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("GrainDialog"));        
    }

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    m_layControls = new QGridLayout;
    m_gbFrame = new QGroupBox(tr("Grain Design"));
    m_gbFrame->setLayout(m_layControls);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(1);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);    
    CylindricalGrain* BatesSeed = dynamic_cast<CylindricalGrain*>(seed);

    //Default to BATES grain design mode
    m_GrainDesign = new CylindricalGrainDesign(m_Propellants, BatesSeed, m_GlobalSettings, m_gbFrame);

    // OK and Cancel buttons
    m_btnApply = new QPushButton(m_gbFrame);
    m_btnCancel = new QPushButton(m_gbFrame);
    m_btnApply->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_btnCancel->setText(tr("Close"));

    m_layControls->addWidget(m_GrainDesign, 1, 0, 3, 3);
    //we use a value of 256 here to ensure that we can have a LOT of extra info padded in there.
    m_layControls->addWidget(m_btnApply, 256, 0);
    m_layControls->addWidget(m_btnCancel, 256, 1);

    QVBoxLayout* masterVLayout = new QVBoxLayout;
    masterVLayout->addWidget(m_gbFrame);
    masterVLayout->addWidget(m_GrainGraphicsView);
    setLayout(masterVLayout);
    OnDesignUpdated();
}
void GrainDialog::SetupGraphics()
{
    m_GrainGraphicsView = new QGraphicsView;
    m_GrainGraphicsScene = new QGraphicsScene;
    m_GrainGraphicsView->setScene(m_GrainGraphicsScene);
    m_GrainGraphicsView->show();
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(true);
    m_GrainGraphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::OnDesignUpdated()
{
    CylindricalGrainDesign* design = dynamic_cast<CylindricalGrainDesign*>(m_GrainDesign);
    if (design)
    {
        if (m_isNewGrainWindow && m_GrainsToEdit.isEmpty())
        {
            OpenBurnGrain* grain = new CylindricalGrain(
                design->GetDiameter(),
                design->GetCoreDiameter(),
                design->GetLength(),
                design->GetPropellant(),                    
                design->GetInhibitedFaces());
            m_GrainsToEdit.push_back(grain);
        }
        else
        {
            for (auto* i : m_GrainsToEdit)
            {
                CylindricalGrain* grain = static_cast<CylindricalGrain*>(i);
                grain->SetDiameter(design->GetDiameter());
                grain->SetCoreDiameter(design->GetCoreDiameter());
                grain->SetLength(design->GetLength());
                grain->SetInhibitedFaces(design->GetInhibitedFaces());
                grain->SetPropellantType(design->GetPropellant());
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
        m_GrainGraphicsScene->addItem(m_gfxGrain);
    }
    m_gfxGrain->setPos(0, 0);    
    m_gfxGrain->update(m_gfxGrain->boundingRect());
    m_GrainGraphicsView->viewport()->repaint();

    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_GrainGraphicsView->setSceneRect(m_gfxGrain->boundingRect());
    QRectF bounds = QRectF(m_gfxGrain->boundingRect().left(), m_gfxGrain->boundingRect().top(), 
        m_gfxGrain->boundingRect().width() + 50, m_gfxGrain->boundingRect().height() + 50);
    m_GrainGraphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_GrainGraphicsView->viewport()->repaint();
    m_gfxGrain->update(m_gfxGrain->boundingRect());    
}
void GrainDialog::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}
void GrainDialog::OnCancelButtonClicked()
{
    close();
}
void GrainDialog::OnApplyButtonClicked()
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
    OnDesignUpdated();
    if (m_isNewGrainWindow)
    {
        emit GrainAdded(m_GrainsToEdit.front());
        m_GrainsToEdit.pop_front();        
    }
    else
    {
        for (auto* i : m_GrainsToEdit)
        {
            emit GrainAdded(i);            
        }    
    }
}
