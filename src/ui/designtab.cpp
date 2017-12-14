#include <QDebug>

#include "src/commands.hpp"
#include "src/ui/designtab.hpp"

DesignTab::DesignTab(OpenBurnMotor* motor, 
	PropellantList* propellantTypes, OpenBurnSettings* settings, 
	QUndoStack* stack, QWidget* parent)
    : QWidget(parent),
	m_UndoStack(stack),
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

    connect(m_GrainTable, &GrainTableWidget::cellDoubleClicked, //Double clicking on a row edits that grain
            this, &DesignTab::OnEditGrainButtonClicked);
    connect(m_GrainTable, &GrainTableWidget::itemSelectionChanged,
        this, &DesignTab::OnTableSelectionChanged);

    connect(m_Motor, &OpenBurnMotor::DesignUpdated,
            this, &DesignTab::OnDesignUpdated);
    UpdateDesign();
}
DesignTab::~DesignTab() 
{
}
void DesignTab::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}

void DesignTab::SetupUI()
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);    

    QHBoxLayout* grainLayout = new QHBoxLayout;
    QVBoxLayout* grainControls = new QVBoxLayout;
    grainLayout->addWidget(m_GrainTable = new GrainTableWidget(m_Motor, m_GlobalSettings, this));

    //controls
    grainControls->addWidget(m_btnNewGrain = new QPushButton(tr("New Grain")));
    m_btnNewGrain->setMinimumHeight(50);
    //add a dividing line
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    grainControls->addSpacing(5);
    grainControls->addWidget(line);
    grainControls->addStretch(10);
    grainControls->addWidget(m_btnEditGrain = new QPushButton(tr("Edit")));
    grainControls->addWidget(m_btnDeleteGrain = new QPushButton(tr("Delete")));
    m_btnEditGrain->setMinimumHeight(30);
    m_btnDeleteGrain->setMinimumHeight(30);

    QHBoxLayout* moveUp = new QHBoxLayout;
    moveUp->addWidget(m_btntMoveGrainUp = new QToolButton);
    m_btntMoveGrainUp->setArrowType(Qt::ArrowType::UpArrow);
    moveUp->addWidget(new QLabel(tr("Move Up")));
    grainControls->addLayout(moveUp);

    QHBoxLayout* moveDown = new QHBoxLayout;
    moveDown->addWidget(m_btntMoveGrainDown = new QToolButton);
    m_btntMoveGrainDown->setArrowType(Qt::ArrowType::DownArrow);
    moveDown->addWidget(new QLabel(tr("Move Down")));
    grainControls->addLayout(moveDown);
    ToggleDesignButtons(false); //disable on startup

    //overall grain design layout
    QGroupBox* gb_GrainDesign = new QGroupBox(tr("Grain Design"));
    grainLayout->addLayout(grainControls);
    grainLayout->setStretch(0, 5);
    grainLayout->setStretch(1, 1);
    gb_GrainDesign->setLayout(grainLayout);

    //nozzle
    m_btnNozzleSettings = new QPushButton(tr("Edit Nozzle"));
    QVBoxLayout* vLayout_2 = new QVBoxLayout;
    vLayout_2->addWidget(m_btnNozzleSettings);
    QGroupBox* gb_frame_Params = new QGroupBox(tr("Chamber Design"));
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

    QVBoxLayout* designLayout = new QVBoxLayout;
    designLayout->addWidget(gb_prop_params);
    designLayout->addWidget(gb_nozz_params);
    designLayout->addWidget(gb_design_params);

    //master layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(gb_GrainDesign, 0, 0, 2, 1);
    layout->addWidget(gb_frame_Params, 0, 1);
    layout->addLayout(designLayout, 1, 1);
    layout->addWidget(m_MotorDisplayView, 2, 0, 1, 2);
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
        " " + lengthUnitSymbol);
    m_lblMotorMajorDia->setText(QString::number(
        m_GlobalSettings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            m_Motor->GetMotorMajorDiameter()), 'f', 2) +
        " " + lengthUnitSymbol);
    m_lblPropellantMass->setText(QString::number(
        m_GlobalSettings->m_MassUnits.ConvertFrom(
            OpenBurnUnits::MassUnits_T::pounds_mass,
            m_Motor->GetMotorPropellantMass() ), 'f', 2) +
        " " + massUnitSymbol);
    m_lblVolumeLoading->setText(QString::number(m_Motor->GetVolumeLoading() * 100.0, 'f', 2) + '%');
    if (m_Motor->HasNozzle())
    {
		m_lblNozzleThroatDia->setText(QString::number(
			m_GlobalSettings->m_LengthUnits.ConvertFrom(
				OpenBurnUnits::LengthUnits_T::inches,
				m_Motor->GetNozzle()->GetNozzleThroat()), 'f', 2) +
			" " + lengthUnitSymbol);
		m_lblNozzleExitDia->setText(QString::number(
			m_GlobalSettings->m_LengthUnits.ConvertFrom(
				OpenBurnUnits::LengthUnits_T::inches,
				m_Motor->GetNozzle()->GetNozzleExit()), 'f', 2) +
			" " + lengthUnitSymbol);
		m_lblNozzleExpansionRatio->setText(QString::number(m_Motor->GetNozzle()->GetNozzleExpansionRatio(), 'f', 2));

        QString initialKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_INITIAL)));
        QString maxKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_MAX)));
        QString finalKn = QString::number(round(m_Motor->CalcStaticKn(KN_CALC_FINAL)));
    
        m_lblKn->setText(initialKn + "-" + maxKn);
        m_lblPortThroatRatio->setText(QString::number(m_Motor->GetPortThroatRatio(), 'f', 2));
    }
	else
	{
		m_lblNozzleThroatDia->setText("");
		m_lblNozzleExitDia->setText("");
		m_lblNozzleExpansionRatio->setText("");
		m_lblKn->setText("");
		m_lblPortThroatRatio->setText("");
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

	//these functions are technically unnecessary as the updating is done behind the scenes with OpenBurnMotor's signals
	if (m_Motor->HasGrains())
	{
		m_gfxMotor->UpdateGrains(m_Motor->GetGrains());
	}
	if (m_Motor->HasNozzle())
	{
		m_gfxMotor->SetNozzle(m_Motor->GetNozzle());
	}
    //set the motor display scene to the middle of the view plus a bit of padding on the sides
	const QRectF rect = m_gfxMotor->boundingRect();
    m_MotorDisplayScene->setSceneRect(rect);
    const QRectF bounds = QRectF(rect.left(), rect.top(), rect.width() + 50, rect.height() + 15);

    m_MotorDisplayView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    repaint();
}

void DesignTab::EditSelectedGrains()
{
    //we want to be able to click "edit" and edit differently selected grains, so we reset the
    //unique_ptr every time the edit button is clicked
    m_GrainDialog.reset();

    m_GrainDialog = std::make_unique<GrainDialog>(m_Propellants,
        m_GrainTable->GetSelectedGrains()[0].get(),
        m_GlobalSettings,
        m_GrainTable->GetSelectedGrains());
    connect(m_GrainDialog.get(), &GrainDialog::GrainsEdited,
        this, &DesignTab::OnGrainsModified);
    connect(m_GrainDialog.get(), &GrainDialog::DialogClosed,
        this, &DesignTab::OnGrainDialogClosed);

    m_GrainDialog->show();
    m_GrainDialog->activateWindow();
    m_GrainDialog->raise();
    m_btnEditGrain->setEnabled(false);

}
void DesignTab::DeleteSelectedGrains()
{
    const auto selected = m_GrainTable->GetSelectedGrains();
    QUndoCommand* removeGrainCommand = new RemoveGrainCommand(selected, m_Motor);
    m_UndoStack->push(removeGrainCommand);

    UpdateDesign();
}
void DesignTab::CreateNewGrain()
{
    if (!m_GrainDialog) //only make one!!
    {
        m_GrainDialog = std::make_unique<GrainDialog>(m_Propellants,
            m_grainSeed.get(),
            m_GlobalSettings);
        connect(m_GrainDialog.get(), &GrainDialog::GrainAdded
            , this, &DesignTab::OnNewGrain);
        connect(m_GrainDialog.get(), &GrainDialog::DialogClosed,
            this, &DesignTab::OnGrainDialogClosed);
    }
    m_GrainDialog->show();
    m_GrainDialog->activateWindow();
    m_GrainDialog->raise();
}

void DesignTab::MoveGrains(bool up)
{
    auto indices = m_GrainTable->GetSelectedGrainIndices();
    while (!indices.empty())
    {
        const int selectedIdx = indices.first();
        indices.pop_front();
        //we should only move the grains up or down if the destination is not outside the bounds of the motor
        const bool shouldNotMove = up ? selectedIdx <= 0 : selectedIdx >= int(m_Motor->GetNumGrains() - 1);
        if (shouldNotMove)
        {
            return;
        }
        m_Motor->SwapGrains(selectedIdx, up ? selectedIdx - 1 : selectedIdx + 1);
    }
    UpdateDesign();
}

void DesignTab::EditNozzle()
{
    if (!m_NozzleDialog) //only make one at a time
    {
        m_NozzleDialog = std::make_unique<NozzleDialog>(m_Motor->GetNozzle(), m_GlobalSettings);
        connect(m_NozzleDialog.get(), &NozzleDialog::NozzleModified,
            this, &DesignTab::OnNozzleUpdated);
        connect(m_NozzleDialog.get(), &NozzleDialog::DialogClosed,
            this, &DesignTab::OnNozzleDialogClosed);
        connect(m_NozzleDialog.get(), &NozzleDialog::NewNozzle,
            this, &DesignTab::OnNewNozzle);
    }
    m_NozzleDialog->show();
    m_NozzleDialog->activateWindow();
    m_NozzleDialog->raise();
}

void DesignTab::ToggleDesignButtons(bool on)
{
    m_btnDeleteGrain->setEnabled(on);
    m_btnEditGrain->setEnabled(on);
    m_btntMoveGrainUp->setEnabled(on);
    m_btntMoveGrainDown->setEnabled(on);
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
void DesignTab::OnNozzleDialogClosed()
{
    m_NozzleDialog.reset();
}
void DesignTab::OnGrainDialogClosed()
{
    m_GrainDialog.reset();
}
void DesignTab::OnNewGrain(const GrainPtr& grain)
{
	QUndoCommand* addCommand = new AddGrainCommand(grain, m_Motor);
	m_UndoStack->push(addCommand);
    SetSeed(grain.get());
}

void DesignTab::OnGrainsModified(const GrainVector& newGrains, const GrainVector& originalGrains)
{
	QUndoCommand* modifyCommand = new ModifyGrainCommand(newGrains, originalGrains, m_Motor);
	m_UndoStack->push(modifyCommand);
	emit m_Motor->DesignUpdated();
	SetSeed(newGrains[0].get());
}

void DesignTab::OnNewNozzle(NozzlePtr& newNozzle)
{
	QUndoCommand* newNozzleCommand = new NewNozzleCommand(std::move(newNozzle), m_Motor);
	m_UndoStack->push(newNozzleCommand);
}

void DesignTab::OnNozzleUpdated(NozzlePtr& newNozzle, NozzlePtr& oldNozzle)
{
	QUndoCommand* nozzleModifyCommand = new ModifyNozzleCommand(std::move(newNozzle), std::move(oldNozzle), m_Motor);
	m_UndoStack->push(nozzleModifyCommand);
}
void DesignTab::OnDesignUpdated()
{
    UpdateDesign();
}
void DesignTab::OnNewGrainButtonClicked()
{
    CreateNewGrain();
}
void DesignTab::OnEditGrainButtonClicked()
{
    EditSelectedGrains();
}
void DesignTab::OnDeleteGrainButtonClicked()
{
    DeleteSelectedGrains();
}
void DesignTab::OnNozzleButtonClicked()
{
    EditNozzle();
}
void DesignTab::OnTableSelectionChanged()
{
    if (!m_GrainTable->selectedItems().empty()) //we have something selected
    {
        OpenBurnGrain* grain = m_GrainTable->GetSelectedGrains()[0].get(); //seed is first grain
        SetSeed(grain);
        ToggleDesignButtons(true);
        emit SelectionChanged(true);
    }
    else
    {
        ToggleDesignButtons(false);
        emit SelectionChanged(false);
    }
}

void DesignTab::OnMoveGrainUpButtonClicked()
{
    MoveGrains(true);
}
void DesignTab::OnMoveGrainDownButtonClicked()
{
    MoveGrains(false);
}
