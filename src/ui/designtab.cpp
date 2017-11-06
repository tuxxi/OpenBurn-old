#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>

#include <QDebug>

#include "src/ui/designtab.h"

DesignTab::DesignTab(OpenBurnMotor* motor, QWidget* parent)
    : QWidget(parent), m_seed_grain(nullptr), m_grainDialog(nullptr), m_nozzleDialog(nullptr), m_motorObject(nullptr),
    m_Motor(motor)
{   
    SetupUI();
    connect(m_newGrainButton, SIGNAL(clicked()), this, SLOT(NewGrainButton_Clicked()));
    connect(m_nozzleSettingsButton, SIGNAL(clicked()), this, SLOT(NozzleButton_Clicked()));
    connect(m_deleteGrainButton, SIGNAL(clicked()), this, SLOT(DeleteGrainButton_Clicked()));
    connect(m_editGrainButton, SIGNAL(clicked()), this, SLOT(EditGrainButton_Clicked()));
    connect(m_grainTable, SIGNAL(cellClicked(int, int)), this, SLOT(SLOT_grainTable_cellClicked(int, int)));
    //Double clicking on a row edits that grain
    connect(m_grainTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(EditGrainButton_Clicked()));
    connect(m_moveGrainUp, SIGNAL(clicked()), this, SLOT(MoveGrainUpButton_Clicked()));
    connect(m_moveGrainDown, SIGNAL(clicked()), this, SLOT(MoveGrainDownButton_Clicked()));
    UpdateDesign();
}
DesignTab::~DesignTab() 
{
    delete m_seed_grain;
}
void DesignTab::SetupUI()
{
    //controls 
    QGroupBox* gb_GrainDesign = new QGroupBox(tr("Grain Design"));
    QGroupBox* gb_frame_Params = new QGroupBox(tr("Chamber Design"));
    QGroupBox* gb_design_overview = new QGroupBox(tr("Design Overview"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);    

    m_grainTable = new GrainTableWidget(m_Motor, this);
    //grain buttons
    m_newGrainButton = new QPushButton(tr("New Grain"));
    m_deleteGrainButton = new QPushButton(tr("Delete"));
    m_editGrainButton = new QPushButton(tr("Edit"));

    m_moveGrainUp = new QToolButton;
    m_moveGrainUp->setArrowType(Qt::ArrowType::UpArrow);
    m_moveGrainDown = new QToolButton;
    m_moveGrainDown->setArrowType(Qt::ArrowType::DownArrow);
    /*
    m_moveGrainToTop = new QToolButton;
    m_moveGrainToTop->setArrowType(Qt::ArrowType::RightArrow);
    m_moveGrainToBottom = new QToolButton;
    m_moveGrainToBottom->setArrowType(Qt::ArrowType::LeftArrow);
    */
    m_deleteGrainButton->setEnabled(false);
    m_editGrainButton->setEnabled(false);
    m_moveGrainUp->setEnabled(false);
    m_moveGrainDown->setEnabled(false);
    //m_moveGrainToTop->setEnabled(false);
    //m_moveGrainToBottom->setEnabled(false);

    QGridLayout* gLayout = new QGridLayout;    
    gLayout->addWidget(m_newGrainButton, 0, 1, 4, 4);
    gLayout->addWidget(m_deleteGrainButton, 0, 0);
    gLayout->addWidget(m_editGrainButton, 1, 0);

    gLayout->addWidget(m_moveGrainUp, 2, 0);
    gLayout->addWidget(m_moveGrainDown, 3, 0);
    //gLayout->addWidget(m_moveGrainToTop, 4, 0);
    //gLayout->addWidget(m_moveGrainToBottom, 5, 0);
    
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
    QGroupBox* gb_prop_params = new QGroupBox(tr("Propellant Parameters"));
    QGroupBox* gb_nozz_params = new QGroupBox(tr("Nozzle Parameters")); 
    QGridLayout* gl_Motor = new QGridLayout;    
    QGridLayout* gl_Propellant = new QGridLayout;
    QGridLayout* gl_Nozzle = new QGridLayout;

    gl_Motor->addWidget(new QLabel(tr("Kn:")), 0, 0);
    gl_Motor->addWidget(m_knLabel = new QLabel, 0, 1);
    gl_Motor->addWidget(new QLabel(tr("Port/Throat Ratio:")), 2, 0);
    gl_Motor->addWidget(m_portThroatRatioLabel = new QLabel, 2, 1);

    gl_Propellant->addWidget(new QLabel(tr("Number of Segments:")), 0, 0);
    gl_Propellant->addWidget(m_numGrainsLabel = new QLabel, 0, 1);
    gl_Propellant->addWidget(new QLabel(tr("Motor Diameter:")), 1, 0);
    gl_Propellant->addWidget(m_motorMajorDiaLabel = new QLabel, 1, 1);
    gl_Propellant->addWidget(new QLabel(tr("Motor Length:")), 2, 0);
    gl_Propellant->addWidget(m_motorLenLabel = new QLabel, 2, 1);
    gl_Propellant->addWidget(new QLabel(tr("Propellant Mass:")), 3, 0);
    gl_Propellant->addWidget(m_propellantMassLabel = new QLabel, 3, 1);
    gl_Propellant->addWidget(new QLabel(tr("Volume Loading:")), 4, 0);
    gl_Propellant->addWidget(m_VolumeLoadingLabel = new QLabel, 4, 1);

    gl_Nozzle->addWidget(new QLabel(tr("Nozzle Throat Diameter:")), 0, 0);
    gl_Nozzle->addWidget(m_nozzleDiaLabel = new QLabel, 0, 1);
    gl_Nozzle->addWidget(new QLabel(tr("Nozzle Exit Diameter:")), 1, 0);
    gl_Nozzle->addWidget(m_nozzleExitLabel = new QLabel, 1, 1);
    
    gb_design_params->setLayout(gl_Motor);
    gb_nozz_params->setLayout(gl_Nozzle);
    gb_prop_params->setLayout(gl_Propellant);
    
    //QGroupBox* gb_sim_quick_results = new QGroupBox(tr("Sim Results"));
    //QGridLayout* gridLayoutSimResults = new QGridLayout;

    QGridLayout* designLayout = new QGridLayout;
    designLayout->addWidget(gb_design_params, 0, 2, 1, 1);
    designLayout->addWidget(gb_nozz_params, 0, 1, 1, 1);
    designLayout->addWidget(gb_prop_params, 0, 0, 1, 1);
    
    designLayout->addWidget(m_motorDisplayView, 1, 0, 1, 3);
    gb_design_overview->setLayout(designLayout);

    //master layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(m_grainTable, 0, 0);
    layout->addWidget(gb_GrainDesign, 0, 1);
    layout->addWidget(gb_frame_Params, 0, 2);
    layout->addWidget(gb_design_overview, 1, 0, 1, 3);
    //takes up 1 row, and two columns
    setLayout(layout);
}

//this MUST be called AFTER we setup the basic UI layout.
void DesignTab::UpdateDesign()
{
    if (m_Motor->HasGrains())
    {        
        m_motorLenLabel->setText(QString::number(m_Motor->GetMotorLength()));
        m_motorMajorDiaLabel->setText(QString::number(m_Motor->GetMotorMajorDiameter()));
        m_numGrainsLabel->setText(QString::number(m_Motor->GetNumGrains()));
        m_propellantMassLabel->setText(QString::number(m_Motor->GetMotorPropellantMass(), 'g', 2));
        m_VolumeLoadingLabel->setText(QString::number(m_Motor->GetVolumeLoading() * 100.f, 'g', 2) + '%');
        if (m_Motor->HasNozzle())
        {
            QString initialKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_INITIAL)));
            QString maxKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_MAX)));
            QString finalKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_FINAL)));
    
            m_knLabel->setText(initialKn + "-" + maxKn);
            m_portThroatRatioLabel->setText(QString::number(m_Motor->GetPortThroatRatio(), 'g', 2));
            emit(m_Motor->SIG_DesignReady()); //design is ready, so anyone who is listening knows they can use it now!
        }
    }
    if (m_Motor->HasNozzle())
    {
        m_nozzleDiaLabel->setText(QString::number(m_Motor->GetNozzle()->GetNozzleThroat()));
        m_nozzleExitLabel->setText(QString::number(m_Motor->GetNozzle()->GetNozzleExit()));
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
    if (m_Motor->HasGrains())
    {
        //ugly hack: it's easier to just remove everything and add all the grains back than it is to try
        //to shift everything around and make the length / width work correctly.... 
        m_motorObject->RemoveAllGrains();
        m_motorObject->SetGrains(m_Motor->GetGrains());
    }
    else
    {
        m_motorObject->RemoveAllGrains();        
    }
    if (m_Motor->HasNozzle())
    {
        m_motorObject->SetNozzle(m_Motor->GetNozzle());        
    }

    //we have to repaint the veiwport otherwise the graphics objects don't update properly
    //m_motorObject->update(m_motorObject->boundingRect());
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
// allocates a new ptr owned by *this so we can store "seed" values,
// even if the grain used to set the seed values is deleted by the user.
void DesignTab::SetSeed(OpenBurnGrain* seed)
{
    BatesGrain* bates = dynamic_cast<BatesGrain*>(seed);
    
    if (m_seed_grain)
    {
        delete m_seed_grain;
        m_seed_grain = nullptr;
    }
    if (bates)
    {
        m_seed_grain = new BatesGrain(*bates);
    }
}
//this allows us to mark the objects as null when they are destroyed, allowing new ones to be made later on
void DesignTab::SLOT_NozzDialogClosed()
{
    m_nozzleDialog = nullptr;
    qDebug() << "nozzle dialog deleted";
}
void DesignTab::SLOT_GrainDialogClosed()
{
    m_grainDialog = nullptr;
    qDebug() << "grain dialog deleted";
}
//Recieved from the grain dialog. Updates the grain table widget
void DesignTab::SLOT_NewGrain(OpenBurnGrain* grain)
{
    m_grainTable->AddNewGrain(grain);
    SetSeed(grain);    
    UpdateDesign();
}
void DesignTab::SLOT_ModifyGrain(OpenBurnGrain* grain)
{
    SetSeed(grain);
    for (size_t idx = 0; idx < m_Motor->GetNumGrains(); idx++)
    {
        if (grain == m_Motor->GetGrains()[idx])
        {
            m_grainTable->ModifyGrain(grain, idx);
            UpdateDesign();
            break;   
        }
    }
}
void DesignTab::SLOT_NozzleUpdated(OpenBurnNozzle* nozz)
{
    m_Motor->SetNozzle(nozz);    
    UpdateDesign();
}
void DesignTab::NewGrainButton_Clicked()
{
    if (!m_grainDialog) //only make one!!
    {
        m_grainDialog = new GrainDialog(nullptr, m_seed_grain);
        connect(m_grainDialog, SIGNAL(SIG_DIALOG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_NewGrain(OpenBurnGrain*)));
        connect(m_grainDialog, SIGNAL(destroyed()), this, SLOT(SLOT_GrainDialogClosed()));
    }
    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise();
}
void DesignTab::EditGrainButton_Clicked()
{
    //we want to be able to click "edit" and edit differently selected grains, so we delete this every time
    //the edit button is clicked
    if (m_grainDialog)
    {
        m_grainDialog->deleteLater();
    }
    m_grainDialog = new GrainDialog(nullptr, m_seed_grain, m_grainTable->GetSelectedGrains());
    connect(m_grainDialog, SIGNAL(SIG_DIALOG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_ModifyGrain(OpenBurnGrain*)));
    connect(m_grainDialog, SIGNAL(destroyed()), this, SLOT(SLOT_GrainDialogClosed()));

    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise();
    m_editGrainButton->setEnabled(false);   
}
void DesignTab::DeleteGrainButton_Clicked()
{
    m_grainTable->DeleteSelectedGrains();

    //refacor me plz :( this is very bad
    
    //disable the button again since we no longer have anything selected
    m_deleteGrainButton->setEnabled(false);
    UpdateDesign();
}
void DesignTab::NozzleButton_Clicked()
{
    if (!m_nozzleDialog) //only make one!!
    {
        m_nozzleDialog = new NozzleDialog(nullptr, m_Motor->GetNozzle());
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
    m_editGrainButton->setEnabled(true);
    m_moveGrainUp->setEnabled(true);
    m_moveGrainDown->setEnabled(true);
}
void DesignTab::MoveGrainUpButton_Clicked()
{
    m_grainTable->move(true);
    UpdateDesign();
}
void DesignTab::MoveGrainDownButton_Clicked()
{
    m_grainTable->move(false);
    UpdateDesign();
}
