#include <QGroupBox>
#include <QGridLayout>
#include <QPen>
#include <QFrame>

#include "simtab.hpp"
#include "src/units.hpp"

static const double pressureMultiplier = 1.10;
static const double motorLenMultiplier = 100;

SimulationTab::SimulationTab(OpenBurnMotor* motor, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent)
    : QWidget(parent),
    m_gfxMotor(nullptr),
    m_currentXPos(0),
    m_animation(nullptr),
    m_lineGraphTime(nullptr),
    m_Motor(motor),
    m_Simulator(sim),
    m_GlobalSettings(settings),
    m_SimSettingsDialog(nullptr)
{
    SetupUI();
    m_SimSettings = std::make_unique<MotorSimSettings>();

    connect(m_Motor, &OpenBurnMotor::DesignReady,
            this, &SimulationTab::OnDesignReady);
    connect(m_Motor, &OpenBurnMotor::DesignUpdated,
            this, &SimulationTab::OnDesignUpdated);
    connect(m_btnRunSim, &QPushButton::clicked,
            this, &SimulationTab::OnRunSimButtonClicked);
    connect(m_btnSimSettings, &QPushButton::clicked,
            this, &SimulationTab::OnSimSettingsButtonClicked);
    connect(m_SimSettings.get(), &MotorSimSettings::SettingsChanged,
            this, &SimulationTab::OnSimSettingsChanged);
    connect(m_sldBurnTimeScrubBar, &QSlider::valueChanged,
            this, &SimulationTab::OnMotorSliceChanged);
    connect(m_btnPlayAnimation, &QPushButton::clicked,
            this, &SimulationTab::OnPlayAnimationButtonClicked);
    connect(m_btntToBeginning, &QToolButton::clicked,
            this, &SimulationTab::OnToBeginningButtonClicked);
    connect(m_sldCurrentXPos, &QSlider::valueChanged,
            this, &SimulationTab::OnXPosChanged);
}
SimulationTab::~SimulationTab()
{

}
void SimulationTab::SetupUI()
{
    SetupPlotter();

    m_btnRunSim = new QPushButton(tr("Run Simulation"));
    m_btnRunSim->setEnabled(false);
    m_btnSimSettings = new QPushButton(tr("Simulation Settings"));

    QGroupBox* gb_Simulate = new QGroupBox(tr("Motor Simulation"));
    QGroupBox* gb_Controls = new QGroupBox(tr("Controls"));    
    QGroupBox* gb_Results = new QGroupBox(tr("Results"));
    QGroupBox* gb_CurrentNumbers = new QGroupBox(tr("Current Chamber"));

    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addWidget(m_btnRunSim);
    controlsLayout->addWidget(m_btnSimSettings);
    gb_Controls->setLayout(controlsLayout);

    QGridLayout* resultsLayout = new QGridLayout;
    resultsLayout->addWidget(new QLabel(tr("Max Pc:")), 0, 0);
    resultsLayout->addWidget(m_lblMaxpressure = new QLabel, 0, 1);
    resultsLayout->addWidget(new QLabel(tr("Max Core Mass Flux:")), 1, 0);
    resultsLayout->addWidget(m_lblCoreMassFlux = new QLabel, 1, 1);
    resultsLayout->addWidget(new QLabel(tr("Burn Time:")), 2, 0);
    resultsLayout->addWidget(m_lblBurnTime = new QLabel, 2, 1);
    resultsLayout->addWidget(new QLabel(tr("Total Impulse:")), 3, 0);
    resultsLayout->addWidget(m_lblTotalImpulse = new QLabel, 3, 1);
    resultsLayout->addWidget(new QLabel(tr("Delivered Isp:")), 4, 0);
    resultsLayout->addWidget(m_lblIsp = new QLabel, 4, 1);
    resultsLayout->addWidget(new QLabel(tr("Motor Designation:")), 5, 0);
    resultsLayout->addWidget(m_lblMotorDesignation = new QLabel, 5, 1);

    gb_Results->setLayout(resultsLayout);
    
    m_MotorDisplayView = new QGraphicsView;
    m_MotorDisplayScene = new QGraphicsScene;
    m_MotorDisplayView->setScene(m_MotorDisplayScene);
    m_MotorDisplayView->show();
    m_sldBurnTimeScrubBar = new QSlider(Qt::Horizontal);
    m_sldBurnTimeScrubBar->setValue(0);

    m_btnPlayAnimation = new QPushButton(tr("Burn!"));
    m_btntToBeginning = new QToolButton();
    m_btntToBeginning->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_btntToBeginning);
    buttonsLayout->addWidget(m_btnPlayAnimation);
    UpdateGraphics();

    QGridLayout* chamberLayout = new QGridLayout;
    chamberLayout->addWidget(new QLabel(tr("Pc:")), 0, 0);
    chamberLayout->addWidget(m_lblCurrentPressure = new QLabel, 0, 1);
    chamberLayout->addWidget(new QLabel(tr("Core Mass Flux:")), 1, 0);
    chamberLayout->addWidget(m_lblCurrentCoreMassFlux = new QLabel, 1, 1);
    chamberLayout->addWidget(new QLabel(tr("Thrust:")), 2, 0);
    chamberLayout->addWidget(m_lblCurrentThrust = new QLabel, 2, 1);
    chamberLayout->addWidget(new QLabel(tr("Isp:")), 3, 0);
    chamberLayout->addWidget(m_lblCurrentIsp = new QLabel, 3, 1);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    chamberLayout->addWidget(line, 4, 0, 1, 2); //add a dividing line
    m_sldCurrentXPos = new QSlider(Qt::Horizontal);

    chamberLayout->addWidget(m_sldCurrentXPos, 5, 0, 1, 2);
    chamberLayout->addWidget(new QLabel(tr("Current X Pos:")), 6, 0);
    chamberLayout->addWidget(m_lblXPos = new QLabel, 6, 1);
    chamberLayout->addWidget(new QLabel(tr("Mass Flux:")), 7, 0);
    chamberLayout->addWidget(m_lblXPosMassFlux = new QLabel, 7, 1);
    chamberLayout->addWidget(new QLabel(tr("Mach Number:")), 8, 0);
    chamberLayout->addWidget(m_lblXPosMachNumber = new QLabel, 8, 1);

    gb_CurrentNumbers->setLayout(chamberLayout);

    QGridLayout* gbGridLayout = new QGridLayout;
    gbGridLayout->addWidget(m_Plotter, 0, 0, 2, 1);
    gbGridLayout->addWidget(gb_Controls, 0, 1);
    gbGridLayout->addWidget(gb_Results, 1, 1);
    gbGridLayout->addWidget(m_sldBurnTimeScrubBar, 2, 0);
    gbGridLayout->addLayout(buttonsLayout, 2, 1);
    gbGridLayout->addWidget(m_MotorDisplayView, 3, 0);
    gbGridLayout->addWidget(gb_CurrentNumbers, 3, 1);

    gbGridLayout->setRowStretch(0, 10);
    gbGridLayout->setRowStretch(2, 1);
    gbGridLayout->setRowStretch(3, 4);
    gbGridLayout->setColumnStretch(0, 5);
    gbGridLayout->setColumnStretch(1, 1);

    gb_Simulate->setLayout(gbGridLayout);
    
    QHBoxLayout* masterLayout = new QHBoxLayout;
    masterLayout->addWidget(gb_Simulate);
    setLayout(masterLayout);
}
void SimulationTab::SetupPlotter()
{
    m_Plotter = new QCustomPlot;
    m_Plotter->addGraph();
    m_Plotter->graph(0)->setPen(QPen(Qt::blue));
    m_Plotter->addGraph();
    m_Plotter->graph(1)->setPen(QPen(Qt::darkRed));
    SetGraphNames();

    // give the axes some labels:
    m_Plotter->xAxis->setLabel(tr("Time"));
    m_Plotter->yAxis->setLabel(tr("Pressure"));
    m_Plotter->yAxis->axisRect()->addAxis(QCPAxis::atRight)->setTickLabelColor(Qt::darkRed);
    QCPAxis* massFluxAxis = m_Plotter->yAxis->axisRect()->axis(QCPAxis::atRight, 1);
    massFluxAxis->setLabel("Mass Flux");
    m_Plotter->legend->setVisible(true);
    // now we move the legend from the inset layout of the axis rect into the main grid layout.
    // We create a sub layout so we can generate a small gap between the plot layout cell border
    // and the legend border:
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    m_Plotter->plotLayout()->addElement(1, 0, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, m_Plotter->legend);
    // change the fill order of the legend, so it's filled left to right in columns:
    m_Plotter->legend->setFillOrder(QCPLegend::foColumnsFirst);
    // set legend's row stretch factor very small so it ends up with minimum height:
    m_Plotter->plotLayout()->setRowStretchFactor(1, 0.001);
}
void SimulationTab::SetGraphNames()
{
    const QString pressureUnits = m_GlobalSettings->m_PressureUnits.GetUnitSymbol();
    const QString massFluxUnits = m_GlobalSettings->m_MassFluxUnits.GetUnitSymbol();

    m_Plotter->graph(0)->setName(tr("Chamber Pressure") + "(" + pressureUnits + ")");
    m_Plotter->graph(1)->setName(tr("Core Mass Flux") + "(" + massFluxUnits + ")");
}
void SimulationTab::UpdateSimulation()
{
    m_Simulator->RunSim(m_SimSettings.get());
    UpdateResults();
    UpdatePlotterLine();
    int idx = m_sldBurnTimeScrubBar->value();
    OpenBurnMotor* motor = m_Simulator->GetResult(idx)->motor.get();
    UpdateCurrentChamber(idx);
    UpdateGraphics(motor);
    UpdateCurrentXPos(m_currentXPos);
}
void SimulationTab::UpdateResults()
{
    //update graphs
    UpdatePlotter();

    //update results labels

    const double totalImpulse = m_GlobalSettings->m_ForceUnits.ConvertFrom(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        m_Simulator->GetTotalImpulse());
    const double deliveredIsp = m_Simulator->GetTotalImpulse() / m_Motor->GetMotorPropellantMass(); //lb-sec / lb
    const double maxPressure = m_GlobalSettings->m_PressureUnits.ConvertFrom(
        OpenBurnUnits::PressureUnits_T::psi,
        m_Simulator->GetMaxPressure());
    const double maxMassFlux = m_GlobalSettings->m_MassFluxUnits.ConvertFrom(
        OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in,
        m_Simulator->GetMaxMassFlux());

    const QString pressureUnits = m_GlobalSettings->m_PressureUnits.GetUnitSymbol();
    const QString forceUnits = m_GlobalSettings->m_ForceUnits.GetUnitSymbol();
    const QString massFluxUnits = m_GlobalSettings->m_MassFluxUnits.GetUnitSymbol();

    m_lblMaxpressure->setText(QString::number(round(maxPressure)) + " "  + pressureUnits);
    m_lblCoreMassFlux->setText(QString::number(maxMassFlux, 'g', 3) + " " + massFluxUnits);
    m_lblBurnTime->setText(QString::number(m_Simulator->GetTotalBurnTime(), 'g', 3) + " s");
    m_lblTotalImpulse->setText(QString::number(round(totalImpulse)) +
                               " " + forceUnits + "-" + tr("sec"));
    m_lblIsp->setText(QString::number(deliveredIsp, 'f', 1) + tr(" s"));
    m_lblMotorDesignation->setText(m_Simulator->GetFullMotorDesignation());

    //update gfx thingies
    m_sldBurnTimeScrubBar->setRange(0, int(m_Simulator->GetNumPoints() - 1));
    UpdateGraphics();
}
void SimulationTab::UpdatePlotterLine()
{
    if (m_lineGraphTime == nullptr)
    {
        m_lineGraphTime = new QCPItemLine(m_Plotter);
        m_lineGraphTime->setClipToAxisRect(false);
    }

    double xPos = m_sldBurnTimeScrubBar->value() * m_SimSettings->timeStep;
    bool dummy;
    double yPos = pressureMultiplier * m_Plotter->graph(0)->getValueRange(dummy).upper;
    m_lineGraphTime->start->setCoords(xPos, 0);
    m_lineGraphTime->end->setCoords(xPos, yPos);
    m_Plotter->replot();
}
void SimulationTab::UpdateCurrentChamber(int currentSlice)
{
    if (m_Simulator->GetResultsEmpty())
    {
        return;
    }
    MotorSimDataPoint* data = m_Simulator->GetResult(currentSlice);

    const QString pressureUnits = m_GlobalSettings->m_PressureUnits.GetUnitSymbol();
    const QString forceUnits = m_GlobalSettings->m_ForceUnits.GetUnitSymbol();
    const QString massFluxUnits = m_GlobalSettings->m_MassFluxUnits.GetUnitSymbol();

    const double currentThrust = m_GlobalSettings->m_ForceUnits.ConvertFrom(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        data->thrust);

    const double currentPressure = m_GlobalSettings->m_PressureUnits.ConvertFrom(
        OpenBurnUnits::PressureUnits_T::psi,
        data->pressure);

    const double currentMassFlux = m_GlobalSettings->m_MassFluxUnits.ConvertFrom(
        OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in,
        data->massflux);
    m_lblCurrentThrust->setText(QString::number(currentThrust, 'f', 1) + " " + forceUnits);
    m_lblCurrentPressure->setText(QString::number(currentPressure, 'f', 1) +  " " + pressureUnits);
    m_lblCurrentCoreMassFlux->setText(QString::number(currentMassFlux, 'f', 3) + " " + massFluxUnits);
    m_lblCurrentIsp->setText(QString::number(data->isp, 'f', 2) + " s");
}
void SimulationTab::UpdatePlotter()
{
    if (m_Simulator->GetResultsEmpty())
    {
        return;
    }
    //prepare graphs
    auto numPoints = int(m_Simulator->GetNumPoints());
    const double maxPressure =m_GlobalSettings->m_PressureUnits.ConvertFrom(
        OpenBurnUnits::PressureUnits_T::psi,
        m_Simulator->GetMaxPressure());

    const double maxPressureScale = pressureMultiplier * maxPressure;
    const double massFluxScale = 300.0f;

    QVector<double> time(numPoints), pressure(numPoints), massflux(numPoints);
    for (int i=0; i<numPoints; ++i)
    {
        time[i] = m_Simulator->GetResult(i)->time;
        pressure[i] = m_GlobalSettings->m_PressureUnits.ConvertFrom(
            OpenBurnUnits::PressureUnits_T::psi,
            m_Simulator->GetResult(i)->pressure);
        massflux[i] = massFluxScale * m_Simulator->GetResult(i)->massflux;
    }
    m_Plotter->graph(0)->setData(time, pressure);
    m_Plotter->graph(1)->setData(time, massflux);

    m_Plotter->xAxis->setRange(0.f, m_Simulator->GetTotalBurnTime() + .2f);
    m_Plotter->yAxis->setRange(0, maxPressureScale);

    QCPAxis* massFluxAxis = m_Plotter->yAxis->axisRect()->axis(QCPAxis::atRight, 1);
    massFluxAxis->setRange(0.0f, maxPressureScale / massFluxScale);

    m_Plotter->replot();
    SetGraphNames();
}
void SimulationTab::UpdateGraphics(OpenBurnMotor *motor)
{
    if (m_Simulator->GetResultsEmpty())
    {
        return;
    }
    if (motor == nullptr)
    {
        motor = GetCurrentSliceMotor();
    }
    if (m_gfxMotor == nullptr)
    {
        m_gfxMotor = std::make_unique<MotorGraphicsItem>(100, m_Motor, true);
        connect(m_gfxMotor.get(), &MotorGraphicsItem::MotorXPosSliceUpdated,
            this, &SimulationTab::OnXPosClicked);
        m_MotorDisplayScene->addItem(m_gfxMotor.get());
    }
    if (m_Motor->HasGrains())
    {
        m_gfxMotor->UpdateGrains(m_Motor->GetGrains());
        if (m_Motor->HasNozzle()) m_gfxMotor->SetNozzle(m_Motor->GetNozzle());
    }

    m_gfxMotor->BurnGrains(motor->GetGrains());

    //set the motor display scene to the middle of the view plus a bit of padding on the sides
    const QRectF rect = m_gfxMotor->boundingRect();
    m_MotorDisplayScene->setSceneRect(rect);
    const QRectF bounds = QRectF(rect.left(), rect.top(), rect.width() + 50, rect.height() + 15);
    m_MotorDisplayView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case
    repaint();
}
void SimulationTab::UpdateCurrentXPos(double xpos)
{
    m_MotorDisplayScene->update();

    const QString lengthUnits = m_GlobalSettings->m_LengthUnits.GetUnitSymbol();
    const QString massFluxUnits = m_GlobalSettings->m_MassFluxUnits.GetUnitSymbol();

    const double xSlice = xpos - (m_Motor->GetMotorLength() - GetCurrentSliceMotor()->GetMotorLength());

    const double x = m_GlobalSettings->m_LengthUnits.ConvertFrom(
        OpenBurnUnits::LengthUnits_T::inches,
        xpos);
    const double massFluxAtX = m_GlobalSettings->m_MassFluxUnits.ConvertFrom(
        OpenBurnUnits::MassFluxUnits_T::lbs_sec_sq_in,
        MotorSim::CalcMassFlux(GetCurrentSliceMotor(), xSlice));
    const double machNumberAtX = MotorSim::CalcMachNumber(GetCurrentSliceMotor(), xSlice);

    m_lblXPos->setText(QString::number(x, 'f', 2) + " " + lengthUnits);
    m_lblXPosMassFlux->setText(QString::number(massFluxAtX, 'f', 3) + " " + massFluxUnits);
    m_lblXPosMachNumber->setText(QString::number(machNumberAtX, 'f', 3));
}
void SimulationTab::OnDesignReady()
{
    if (m_GlobalSettings->m_redrawOnChanges)
    {
        UpdateSimulation();
    }
    m_btnRunSim->setEnabled(true);
    m_sldCurrentXPos->setRange(0, static_cast<int>(m_Motor->GetMotorLength() * motorLenMultiplier));
}
void SimulationTab::OnDesignUpdated()
{
    UpdateGraphics();
}
void SimulationTab::OnSimSettingsButtonClicked()
{
    if (m_SimSettingsDialog == nullptr)
    {
        m_SimSettingsDialog = std::make_unique<SimSettingsDialog>(m_SimSettings.get(), m_GlobalSettings);
    }
    m_SimSettingsDialog->SetGlobalSettings(m_GlobalSettings);
    m_SimSettingsDialog->show();
    m_SimSettingsDialog->activateWindow();
    m_SimSettingsDialog->raise();
    m_sldCurrentXPos->setRange(0, static_cast<int>(m_Motor->GetMotorLength() * motorLenMultiplier));
}
void SimulationTab::OnRunSimButtonClicked()
{
    UpdateSimulation();
}
void SimulationTab::OnSimSettingsChanged()
{
    UpdateSimulation();
}
void SimulationTab::OnPlayAnimationButtonClicked()
{
    if (!m_animation)
    {
        m_animation = std::make_unique<QPropertyAnimation>(m_sldBurnTimeScrubBar,"sliderPosition");
        connect(m_animation.get(), &QPropertyAnimation::finished,
            this, &SimulationTab::OnAnimationFinished);
    }
    m_animation->setDuration(static_cast<int>(1000 * m_Simulator->GetTotalBurnTime()));
    m_animation->setStartValue(m_sldBurnTimeScrubBar->minimum());
    m_animation->setEndValue(m_sldBurnTimeScrubBar->maximum());
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->start(); 
}
void SimulationTab::OnToBeginningButtonClicked()
{
    if (m_animation) m_animation->stop();
    m_sldBurnTimeScrubBar->setValue(0);
}
void SimulationTab::OnMotorSliceChanged(int sliceIndex)
{
    if (m_Simulator->GetResultsEmpty()) return;
    OpenBurnMotor* motor = m_Simulator->GetResult(sliceIndex)->motor.get();
    UpdatePlotterLine();
    UpdateCurrentChamber(sliceIndex);
    UpdateGraphics(motor);
    UpdateCurrentXPos(m_currentXPos);
}
void SimulationTab::OnXPosChanged(int xPos)
{
    if (m_gfxMotor)
    {
        m_gfxMotor->SetXPos(double(xPos));
    }
    UpdateCurrentXPos(m_currentXPos);
}
void SimulationTab::OnAnimationFinished()
{
    m_animation.reset();
}
void SimulationTab::OnXPosClicked(double newXPos)
{
    m_currentXPos = newXPos;
    m_sldCurrentXPos->setValue(static_cast<int>(newXPos * motorLenMultiplier));
}

void SimulationTab::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}
OpenBurnMotor* SimulationTab::GetCurrentSliceMotor()
{
    if (m_Simulator->GetResultsEmpty()) 
        return nullptr;
    return m_Simulator->GetResult(m_sldBurnTimeScrubBar->value())->motor.get();
}
