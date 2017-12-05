#include <QSizePolicy>
#include <QGridLayout>
#include <QDebug>

#include "src/ui/designtab.h"

DesignTab::DesignTab(OpenBurnMotor* motor, PropellantList* propellantTypes, OpenBurnSettings* settings, QWidget* parent)
    : QWidget(parent),
      m_grainSeed(nullptr), m_GrainDialog(nullptr), m_NozzleDialog(nullptr), m_gfxMotor(nullptr),
      m_Motor(motor),
      m_Propellants(propellantTypes),
      m_GlobalSettings(settings)
{   
    SetupUI();
    connect(m_btnNewGrain, &QPushButton::clicked,
            this, &DesignTab::OnNewGrainButtonClicked);
    connect(m_btnNozzleSettings, &QPushButton::clicked,
            this, &DesignTab::OnNozzleButtonClicked);
    connect(m_btnDeleteGrain, &QPushButton::clicked,
            this, &DesignTab::OnDeleteGrainButtonClicked);
    connect(m_btnEditGrain, &QPushButton::clicked,
            this, &DesignTab::OnEditGrainButtonClicked);
    connect(m_btntMoveGrainUp, &QPushButton::clicked,
            this, &DesignTab::OnMoveGrainUpButtonClicked);
    connect(m_btntMoveGrainDown, &QPushButton::clicked,
            this, &DesignTab::OnMoveGrainDownButtonClicked);

    connect(m_GrainTable, &GrainTableWidget::cellClicked,
            this, &DesignTab::OnGrainTableCellClicked);
    connect(m_GrainTable, &GrainTableWidget::cellDoubleClicked, //Double clicking on a row edits that grain
            this, &DesignTab::OnEditGrainButtonClicked);
    connect(m_Motor, &OpenBurnMotor::DesignUpdated,
            m_GrainTable, &GrainTableWidget::OnMotorUpdated);
    connect(m_Motor, &OpenBurnMotor::DesignUpdated,
            this, &DesignTab::OnDesignUpdated);
    UpdateDesign();
}
DesignTab::~DesignTab() 
{
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

    m_GrainTable = new GrainTableWidget(m_Motor, m_GlobalSettings, this);
    //grain buttons
    m_btnNewGrain = new QPushButton(tr("New Grain"));
    m_btnDeleteGrain = new QPushButton(tr("Delete"));
    m_btnEditGrain = new QPushButton(tr("Edit"));

    m_btntMoveGrainUp = new QToolButton;
    m_btntMoveGrainUp->setArrowType(Qt::ArrowType::UpArrow);
    m_btntMoveGrainDown = new QToolButton;
    m_btntMoveGrainDown->setArrowType(Qt::ArrowType::DownArrow);
    /*
    m_moveGrainToTop = new QToolButton;
    m_moveGrainToTop->setArrowType(Qt::ArrowType::RightArrow);
    m_moveGrainToBottom = new QToolButton;
    m_moveGrainToBottom->setArrowType(Qt::ArrowType::LeftArrow);
    */
    m_btnDeleteGrain->setEnabled(false);
    m_btnEditGrain->setEnabled(false);
    m_btntMoveGrainUp->setEnabled(false);
    m_btntMoveGrainDown->setEnabled(false);
    //m_moveGrainToTop->setEnabled(false);
    //m_moveGrainToBottom->setEnabled(false);

    QGridLayout* gLayout = new QGridLayout;    
    gLayout->addWidget(m_btnNewGrain, 0, 1, 4, 4);
    gLayout->addWidget(m_btnDeleteGrain, 0, 0);
    gLayout->addWidget(m_btnEditGrain, 1, 0);

    gLayout->addWidget(m_btntMoveGrainUp, 2, 0);
    gLayout->addWidget(m_btntMoveGrainDown, 3, 0);
    //gLayout->addWidget(m_moveGrainToTop, 4, 0);
    //gLayout->addWidget(m_moveGrainToBottom, 5, 0);
    
    gb_GrainDesign->setLayout(gLayout);

    //nozzle and sim settings
    m_btnNozzleSettings = new QPushButton(tr("Edit Nozzle"));
    QVBoxLayout* vLayout_2 = new QVBoxLayout;
    vLayout_2->addWidget(m_btnNozzleSettings);
    gb_frame_Params->setLayout(vLayout_2);

    //design overview
    m_MotorDisplayView = new QGraphicsView;
    m_MotorDisplayScene = new QGraphicsScene;
    m_MotorDisplayView->setScene(m_MotorDisplayScene);
    m_MotorDisplayView->show();

    QGroupBox* gb_design_params = new QGroupBox(tr("Design Parameters"));
    QGroupBox* gb_prop_params = new QGroupBox(tr("Propellant Parameters"));
    QGroupBox* gb_nozz_params = new QGroupBox(tr("Nozzle Parameters")); 
    QGridLayout* gl_Motor = new QGridLayout;    
    QGridLayout* gl_Propellant = new QGridLayout;
    QGridLayout* gl_Nozzle = new QGridLayout;

    gl_Motor->addWidget(new QLabel(tr("Kn:")), 0, 0);
    gl_Motor->addWidget(m_lblKn = new QLabel, 0, 1);
    gl_Motor->addWidget(new QLabel(tr("Port/Throat Ratio:")), 2, 0);
    gl_Motor->addWidget(m_lblPortThroatRatio = new QLabel, 2, 1);

    gl_Propellant->addWidget(new QLabel(tr("Number of Segments:")), 0, 0);
    gl_Propellant->addWidget(m_lblNumGrains = new QLabel, 0, 1);
    gl_Propellant->addWidget(new QLabel(tr("Motor Diameter:")), 1, 0);
    gl_Propellant->addWidget(m_lblMotorMajorDia = new QLabel, 1, 1);
    gl_Propellant->addWidget(new QLabel(tr("Motor Length:")), 2, 0);
    gl_Propellant->addWidget(m_lblMotorLen = new QLabel, 2, 1);
    gl_Propellant->addWidget(new QLabel(tr("Propellant Mass:")), 3, 0);
    gl_Propellant->addWidget(m_lblPropellantMass = new QLabel, 3, 1);
    gl_Propellant->addWidget(new QLabel(tr("Volume Loading:")), 4, 0);
    gl_Propellant->addWidget(m_lblVolumeLoading = new QLabel, 4, 1);

    gl_Nozzle->addWidget(new QLabel(tr("Nozzle Throat Diameter:")), 0, 0);
    gl_Nozzle->addWidget(m_lblNozzleThroatDia = new QLabel, 0, 1);
    gl_Nozzle->addWidget(new QLabel(tr("Nozzle Exit Diameter:")), 1, 0);
    gl_Nozzle->addWidget(m_lblNozzleExitDia = new QLabel, 1, 1);
    gl_Nozzle->addWidget(new QLabel(tr("Expansion Ratio:")), 2, 0);
    gl_Nozzle->addWidget(m_lblNozzleExpansionRatio = new QLabel, 2, 1);
    gb_design_params->setLayout(gl_Motor);
    gb_nozz_params->setLayout(gl_Nozzle);
    gb_prop_params->setLayout(gl_Propellant);
    
    //QGroupBox* gb_sim_quick_results = new QGroupBox(tr("Sim Results"));
    //QGridLayout* gridLayoutSimResults = new QGridLayout;

    QGridLayout* designLayout = new QGridLayout;
    designLayout->addWidget(gb_design_params, 0, 2, 1, 1);
    designLayout->addWidget(gb_nozz_params, 0, 1, 1, 1);
    designLayout->addWidget(gb_prop_params, 0, 0, 1, 1);
    
    designLayout->addWidget(m_MotorDisplayView, 1, 0, 1, 3);
    gb_design_overview->setLayout(designLayout);

    //master layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(m_GrainTable, 0, 0);
    layout->addWidget(gb_GrainDesign, 0, 1);
    layout->addWidget(gb_frame_Params, 0, 2);
    layout->addWidget(gb_design_overview, 1, 0, 1, 3);
    //takes up 1 row, and two columns
    setLayout(layout);
}

//this MUST be called AFTER we setup the basic UI layout.
void DesignTab::UpdateDesign()
{
    const QString lengthUnitSymbol = m_GlobalSettings->m_LengthUnits.GetUnitSymbol();
    const QString massUnitSymbol = m_GlobalSettings->m_MassUnits.GetUnitSymbol();

	m_lblNumGrains->setText(QString::number(m_Motor->GetNumGrains()));
 
    m_lblMotorLen->setText(QString::number(
        m_GlobalSettings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            m_Motor->GetMotorLength()), 'f', 2) +
        " " +
        lengthUnitSymbol);
    m_lblMotorMajorDia->setText(QString::number(
        m_GlobalSettings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            m_Motor->GetMotorMajorDiameter()), 'f', 2) +
        " " +
        lengthUnitSymbol);
    m_lblPropellantMass->setText(QString::number(
        m_GlobalSettings->m_MassUnits.ConvertFrom(
            OpenBurnUnits::MassUnits_T::pounds_mass,
            m_Motor->GetMotorPropellantMass() ), 'f', 2) +
        " " +
        massUnitSymbol);
    m_lblVolumeLoading->setText(QString::number(m_Motor->GetVolumeLoading() * 100.f, 'f', 2) + '%');
    if (m_Motor->HasNozzle())
    {
		m_lblNozzleThroatDia->setText(QString::number(
			m_GlobalSettings->m_LengthUnits.ConvertFrom(
				OpenBurnUnits::LengthUnits_T::inches,
				m_Motor->GetNozzle()->GetNozzleThroat()), 'f', 2) +
			" " +
			lengthUnitSymbol);
		m_lblNozzleExitDia->setText(QString::number(
			m_GlobalSettings->m_LengthUnits.ConvertFrom(
				OpenBurnUnits::LengthUnits_T::inches,
				m_Motor->GetNozzle()->GetNozzleExit()), 'f', 2) +
			" " +
			lengthUnitSymbol);
		m_lblNozzleExpansionRatio->setText(QString::number(m_Motor->GetNozzle()->GetNozzleExpansionRatio(), 'f', 2));

        QString initialKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_INITIAL)));
        QString maxKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_MAX)));
        QString finalKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_FINAL)));
    
        m_lblKn->setText(initialKn + "-" + maxKn);
        m_lblPortThroatRatio->setText(QString::number(m_Motor->GetPortThroatRatio(), 'f', 2));
    }
    
    UpdateGraphics(); 
}
void DesignTab::UpdateGraphics()
{
    if (m_gfxMotor == nullptr)
    {
		m_gfxMotor = std::make_unique<MotorGraphicsItem>(100, m_Motor);
        m_MotorDisplayScene->addItem(m_gfxMotor.get());
    }

	if (m_Motor->HasGrains())
	{
		m_gfxMotor->UpdateGrains(m_Motor->GetGrains());
		if (m_Motor->HasNozzle()) m_gfxMotor->SetNozzle(m_Motor->GetNozzle());
	}

    //set the motor display scene to the middle of the view plus a bit of padding on the sides
	const QRectF rect = m_gfxMotor->boundingRect();
    m_MotorDisplayScene->setSceneRect(rect);
    const QRectF bounds = QRectF(rect.left(), rect.top(), rect.width() + 50, rect.height() + 15);

    m_MotorDisplayView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    repaint();
}
void DesignTab::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}
// allocates a new ptr owned by *this so we can store "seed" values
void DesignTab::SetSeed(OpenBurnGrain* seed)
{
	m_grainSeed.reset();
    if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(seed))
    {
		m_grainSeed = std::make_unique<CylindricalGrain>(*bates);
    }
}
//this allows us to mark the objects as null when they are destroyed (via attribute WA_DeleteOnClose),
//allowing a new object to be made the next time we need one
void DesignTab::OnNozzleDialogClosed()
{
    m_NozzleDialog = nullptr;
}
void DesignTab::OnGrainDialogClosed()
{
    m_GrainDialog = nullptr;
}

void DesignTab::OnNewGrain(const std::shared_ptr<OpenBurnGrain>& grain)
{
    m_Motor->AddGrain(std::move(grain));
    SetSeed(grain.get());
}
void DesignTab::OnGrainModified(const std::shared_ptr<OpenBurnGrain>& grain)
{
    emit m_Motor->DesignUpdated();
    SetSeed(grain.get());
}
void DesignTab::OnNozzleUpdated(OpenBurnNozzle* nozz)
{
    m_Motor->SetNozzle(nozz);  
}
void DesignTab::OnDesignUpdated()
{
    UpdateDesign();
}
void DesignTab::OnNewGrainButtonClicked()
{
    if (!m_GrainDialog) //only make one!!
    {
        m_GrainDialog = new GrainDialog(m_Propellants,
                m_grainSeed.get(),
                m_GlobalSettings);
        connect(m_GrainDialog, &GrainDialog::GrainAdded
			, this, &DesignTab::OnNewGrain);
        connect(m_GrainDialog, &GrainDialog::destroyed, 
			this, &DesignTab::OnGrainDialogClosed);
    }
    m_GrainDialog->show();
    m_GrainDialog->activateWindow();
    m_GrainDialog->raise();
}
void DesignTab::OnEditGrainButtonClicked()
{
    //we want to be able to click "edit" and edit differently selected grains, so we delete this every time
    //the edit button is clicked
	
	if (m_GrainDialog)
	{
		delete m_GrainDialog;
		m_GrainDialog = nullptr;
	}
    m_GrainDialog = new GrainDialog(m_Propellants,
            m_GrainTable->GetSelectedGrains()[0].get(),
            m_GlobalSettings,
            m_GrainTable->GetSelectedGrains());
	connect(m_GrainDialog, &GrainDialog::GrainEdited
		, this, &DesignTab::OnGrainModified);
	connect(m_GrainDialog, &GrainDialog::destroyed,
		this, &DesignTab::OnGrainDialogClosed);

    m_GrainDialog->show();
    m_GrainDialog->activateWindow();
    m_GrainDialog->raise();
    m_btnEditGrain->setEnabled(false);
}
void DesignTab::OnDeleteGrainButtonClicked()
{
    QList<int> selected = m_GrainTable->GetSelectedGrainIndices();
    int count = 0;
    for (auto i : selected)
    {
        m_Motor->RemoveGrain(i - count);
        count++;
    }
    //disable the button again since we no longer have anything selected
	ToggleDesignButtons(false);
    UpdateDesign();
}
void DesignTab::OnNozzleButtonClicked()
{
    if (!m_NozzleDialog) //only make one!!
    {
        m_NozzleDialog = new NozzleDialog(nullptr, m_Motor->GetNozzle(), m_GlobalSettings);
        connect(m_NozzleDialog, SIGNAL(NozzleChanged(OpenBurnNozzle*)), this, SLOT(OnNozzleUpdated(OpenBurnNozzle*)));
        connect(m_NozzleDialog, SIGNAL(destroyed()), this, SLOT(OnNozzleDialogClosed()));
    }
    m_NozzleDialog->show();
    m_NozzleDialog->activateWindow();
    m_NozzleDialog->raise();
}
void DesignTab::OnGrainTableCellClicked(int row, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(row);
    SetSeed(m_Motor->GetGrains()[row].get());
	ToggleDesignButtons(true);
}

void DesignTab::ToggleDesignButtons(bool on)
{
	m_btnDeleteGrain->setEnabled(on);
	m_btnEditGrain->setEnabled(on);
	m_btntMoveGrainUp->setEnabled(on);
	m_btntMoveGrainDown->setEnabled(on);
}
void DesignTab::OnMoveGrainUpButtonClicked()
{
    QList<int> indices = m_GrainTable->GetSelectedGrainIndices();
    while (!indices.empty())
    {
        int selectedIdx = indices.first();  
        indices.pop_front();   
        if (selectedIdx <= 0)   
        {
            return;
        }
        else
        {
            m_Motor->SwapGrains(selectedIdx, selectedIdx-1);
        }
    }
    UpdateDesign();
}
void DesignTab::OnMoveGrainDownButtonClicked()
{
    UpdateDesign();
    QList<int> indices = m_GrainTable->GetSelectedGrainIndices();
    while (!indices.empty())
    {
        int selectedIdx = indices.last();  
        indices.pop_front();   
        if (selectedIdx >= int(m_Motor->GetNumGrains() - 1))   
        {
            return;
        }
        else
        {
            m_Motor->SwapGrains(selectedIdx, selectedIdx+1);
        }
    }
    UpdateDesign();
}
