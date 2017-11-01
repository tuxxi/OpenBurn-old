#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>

#include <QDebug>

#include "src/ui/designtab.h"

DesignTab::DesignTab(QWidget* parent)
    : QWidget(parent), m_grainDialog(nullptr), m_nozzleDialog(nullptr), m_motorObject(nullptr)
{   
    SetupUI();
    connect(m_newGrainButton, SIGNAL(clicked()), this, SLOT(NewGrainButton_Clicked()));
    connect(m_nozzleSettingsButton, SIGNAL(clicked()), this, SLOT(NozzleButton_Clicked()));
    connect(m_grainsDisplay, SIGNAL(SIG_GrainPositionUpdated(int, int)),
        this, SLOT(SLOT_GrainPositionUpdated(int, int)));
    connect(m_deleteGrainButton, SIGNAL(clicked()), this, SLOT(DeleteGrainButton_Clicked()));
    connect(m_grainsDisplay, SIGNAL(cellClicked(int, int)), this, SLOT(SLOT_grainTable_cellClicked(int, int)));
    m_sim = new MotorSim;
    UpdateDesign();
}
DesignTab::~DesignTab() 
{
    delete m_sim;
}
void DesignTab::SetupUI()
{
    //controls 
    QGroupBox* gb_GrainDesign = new QGroupBox(tr("Grain Design"));
    QGroupBox* gb_frame_Params = new QGroupBox(tr("Simulation Settings"));
    QGroupBox* gb_design_overview = new QGroupBox(tr("Design Overview"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);    

    m_grainsDisplay = new GrainTableWidget(this);
    //grain buttons
    m_newGrainButton = new QPushButton(tr("New Grain"));
    m_deleteGrainButton = new QPushButton(tr("Delete"));
    m_deleteGrainButton->setEnabled(false);
    QGridLayout* gLayout = new QGridLayout;    
    gLayout->addWidget(m_newGrainButton, 0, 1, 2, 2);
    gLayout->addWidget(m_deleteGrainButton, 0, 0);
    gb_GrainDesign->setLayout(gLayout);

    //nozzle and sim settings
    m_nozzleSettingsButton = new QPushButton(tr("Edit Nozzle"));
    QVBoxLayout* vLayout_2 = new QVBoxLayout;
    vLayout_2->addWidget(m_nozzleSettingsButton);
    gb_frame_Params->setLayout(vLayout_2);

    //design overview
    m_motorDisplayView = new QGraphicsView;
    m_motorDisplayScene = new QGraphicsScene;
    m_motorDisplayView->setScene(m_motorDisplayScene);
    m_motorDisplayView->show();

    QGroupBox* gb_design_params = new QGroupBox(tr("Design Parameters"));
    QGridLayout* gridLayoutDesignParams = new QGridLayout;
    m_knLabel = new QLabel;
    m_nozzleDiaLabel = new QLabel;
    m_nozzleExitLabel = new QLabel;
    m_motorMajorDiaLabel = new QLabel;
    m_motorLenLabel = new QLabel;
    m_numGrainsLabel = new QLabel;
    gridLayoutDesignParams->addWidget(new QLabel(tr("Kn:")), 0, 0);
    gridLayoutDesignParams->addWidget(m_knLabel, 0, 1);
    gridLayoutDesignParams->addWidget(new QLabel(tr("Nozzle Throat Diameter:")), 1, 0);
    gridLayoutDesignParams->addWidget(m_nozzleDiaLabel, 1, 1);
    gridLayoutDesignParams->addWidget(new QLabel(tr("Nozzle Exit Diameter:")), 2, 0);
    gridLayoutDesignParams->addWidget(m_nozzleExitLabel, 2, 1);
    gridLayoutDesignParams->addWidget(new QLabel(tr("Grains:")), 3, 0);
    gridLayoutDesignParams->addWidget(m_numGrainsLabel, 3, 1);
    gridLayoutDesignParams->addWidget(new QLabel(tr("Motor Diameter:")), 4, 0);
    gridLayoutDesignParams->addWidget(m_motorMajorDiaLabel, 4, 1);
    gridLayoutDesignParams->addWidget(new QLabel(tr("Motor Length:")), 5, 0);
    gridLayoutDesignParams->addWidget(m_motorLenLabel, 5, 1);

    gb_design_params->setLayout(gridLayoutDesignParams);

    //QGroupBox* gb_sim_quick_results = new QGroupBox(tr("Sim Results"));
    //QGridLayout* gridLayoutSimResults = new QGridLayout;

    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(m_motorDisplayView);
    hBoxLayout->addWidget(gb_design_params);
    gb_design_overview->setLayout(hBoxLayout);

    //master layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(m_grainsDisplay, 0, 0);
    layout->addWidget(gb_GrainDesign, 0, 1);
    layout->addWidget(gb_frame_Params, 0, 2);
    layout->addWidget(gb_design_overview, 1, 0, 1, 3);
    //takes up 1 row, and two columns
    setLayout(layout);
}

//this MUST be called AFTER we setup the basic UI layout.
void DesignTab::UpdateDesign()
{
    if (m_sim->HasGrains())
    {
        m_motorObject->SetGrains(m_sim->m_Grains);
        m_motorLenLabel->setText(num(m_sim->GetMotorLength()));
        m_motorMajorDiaLabel->setText(num(m_sim->GetMotorMajorDiameter()));
        m_numGrainsLabel->setText(num(m_sim->GetNumGrains()));

        if (m_sim->HasNozzle())
        {
            m_motorObject->SetNozzle(m_sim->m_Nozzle);
            QString initialKn = num(MotorSim::CalcStaticKn(m_sim->m_Grains, m_sim->m_Nozzle, KN_CALC_INITIAL));
            QString maxKn = num(MotorSim::CalcStaticKn(m_sim->m_Grains, m_sim->m_Nozzle, KN_CALC_MAX));
            QString finalKn = num(MotorSim::CalcStaticKn(m_sim->m_Grains, m_sim->m_Nozzle, KN_CALC_FINAL));
    
            m_knLabel->setText(initialKn + "-" + maxKn);    
        }
    }
    if (m_sim->HasNozzle())
    {
        m_nozzleDiaLabel->setText(num(m_sim->m_Nozzle->GetNozzleThroat()));
        m_nozzleExitLabel->setText(num(m_sim->m_Nozzle->GetNozzleExit()));
    }
    UpdateGraphics(); 
}
void DesignTab::UpdateGraphics()
{
    if (!m_motorObject)
    {
        m_motorObject = new MotorGraphicsItem(100);
        m_motorDisplayScene->addItem(m_motorObject);
    }

    //we have to repaint the veiwport otherwise the graphics objects don't update properly
    m_motorObject->update(m_motorObject->boundingRect());
    m_motorDisplayView->viewport()->repaint();

    //set the motor display scene to the middle of the view plus a bit of padding on the sides
    m_motorDisplayScene->setSceneRect(m_motorObject->boundingRect());
    QRectF bounds = QRectF(m_motorObject->boundingRect().left(), m_motorObject->boundingRect().top(), 
        m_motorObject->boundingRect().width() + 50, m_motorObject->boundingRect().height() + 15);
    m_motorDisplayView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_motorDisplayView->viewport()->repaint();    
}
void DesignTab::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}
//this allows us to mark the objects as null when they are destroyed, allowing new ones to be made later on
void DesignTab::SLOT_NozzDialogClosed()
{
    m_nozzleDialog = nullptr;
}
void DesignTab::SLOT_GrainDialogClosed()
{
    m_grainDialog = nullptr;
    qDebug() << "deleted grain dialog!";
}

//Recieved from the grain dialog. Updates the grain table widget,
//and send sout a signal to main window that there was a new grain added
void DesignTab::SLOT_NewGrain(OpenBurnGrain* grain)
{
    int numItems = m_grainsDisplay->rowCount();
    m_grainsDisplay->setRowCount(numItems+1);

    m_grainsDisplay->setItem(numItems, 0, new QTableWidgetItem(num(grain->GetLength())));
    m_grainsDisplay->setItem(numItems, 1, new QTableWidgetItem(num(grain->GetDiameter())));
    m_grainsDisplay->setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType()->GetPropellantName()));
    m_grainsDisplay->setItem(numItems, 4, new QTableWidgetItem(num(grain->GetInhibitedFaces())));

    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(grain))
    {
        m_grainsDisplay->setItem(numItems, 2, new QTableWidgetItem(num(bates->GetCoreDiameter())));
    }

    m_sim->AddGrain(grain);
    emit SIG_NewGrain(grain);
    UpdateDesign();
}
//Recieved from the grain table widget. Update the sim!
void DesignTab::SLOT_GrainPositionUpdated(int oldPos, int newPos)
{
    m_sim->SwapGrains(oldPos, newPos);
    UpdateDesign();
}
void DesignTab::SLOT_NozzleUpdated(OpenBurnNozzle* nozz)
{
    m_sim->SetNozzle(nozz);    
    UpdateDesign();    
}
void DesignTab::NewGrainButton_Clicked()
{
    if (!m_grainDialog) //only make one!!
    {
        m_grainDialog = new GrainDialog;
        connect(m_grainDialog, SIGNAL(SIG_DIALOG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_NewGrain(OpenBurnGrain*)));
        connect(m_grainDialog, SIGNAL(destroyed()), this, SLOT(SLOT_GrainDialogClosed()));
        
    }
    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise();
}
void DesignTab::DeleteGrainButton_Clicked()
{
    for (int i = 0; i <= m_grainsDisplay->rowCount(); i++)
    {
        if (m_grainsDisplay->isItemSelected(m_grainsDisplay->item(i, 0)))
        {
            //qDebug() << "grain # " << i << "deleted.";
            m_grainsDisplay->removeRow(i);
            m_sim->RemoveGrain(i);
            m_motorObject->RemoveGrain(i);
        }
    }
    //disable the button again since we no longer have anything selected
    m_deleteGrainButton->setEnabled(false);
    UpdateDesign();
}
void DesignTab::NozzleButton_Clicked()
{
    if (!m_nozzleDialog) //only make one!!
    {
        m_nozzleDialog = new NozzleDialog;
        connect(m_nozzleDialog, SIGNAL(SIG_NozzleChanged(OpenBurnNozzle*)), this, SLOT(SLOT_NozzleUpdated(OpenBurnNozzle*)));
        connect(m_nozzleDialog, SIGNAL(destroyed()), this, SLOT(SLOT_NozzDialogClosed()));
    }
    m_nozzleDialog->show();
    m_nozzleDialog->activateWindow();
    m_nozzleDialog->raise();
}
void DesignTab::SLOT_grainTable_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(row);
    //m_seed_grain = m_sim->m_Grains[row];
    m_deleteGrainButton->setEnabled(true);
}
