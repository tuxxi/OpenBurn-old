#include <QGroupBox>
#include <QGridLayout>

#include <cmath>

#include "simtab.h"
#include "src/units.h"

static const QString massFluxUnits = QObject::tr("lbs/sec/inch²"); //todo: make more mass flux units?

SimulationTab::SimulationTab(OpenBurnMotor* motor, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent)
    : QWidget(parent),
      m_Motor(motor),
      m_Simulator(sim),
      m_GlobalSettings(settings),
      m_SimSettingsDialog(nullptr)
{
    SetupUI();
    connect(m_Motor, &OpenBurnMotor::DesignReady,
            this, &SimulationTab::OnDesignReady);
    connect(m_btnRunSim, &QPushButton::clicked,
            this, &SimulationTab::OnRunSimButtonClicked);
    connect(m_btnSimSettings, &QPushButton::clicked,
            this, &SimulationTab::OnSimSettingsButtonClicked);
    m_SimSettings = new MotorSimSettings;
    connect(m_SimSettings, &MotorSimSettings::SettingsChanged,
            this, &SimulationTab::OnSimSettingsChanged);
}
SimulationTab::~SimulationTab()
{
    delete m_SimSettings;
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
    
    QGridLayout* gbGridLayout = new QGridLayout;    
    gbGridLayout->addWidget(m_Plotter, 0, 0, 2, 3);
    gbGridLayout->addWidget(gb_Controls, 0, 3, 1, 1);    
    gbGridLayout->addWidget(gb_Results, 1, 3, 1, 1);
    //gbGridLayout->addWidget(m_RunSimulationButton, 0, 3, 1, 1);
    gb_Simulate->setLayout(gbGridLayout);
    
    QHBoxLayout* masterLayout = new QHBoxLayout;
    masterLayout->addWidget(gb_Simulate);
    setLayout(masterLayout);
}
void SimulationTab::UpdateSimulation()
{
    m_Simulator->RunSim(m_SimSettings);
    UpdateResults();
}
void SimulationTab::UpdateResults()
{
    //update graphs
    UpdatePlotter();

    //update results labels
    const double totalImpulseN = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        m_Simulator->GetTotalImpulse());
    const double avgThrustN = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        m_Simulator->GetAvgThrust());
    const double totalImpulse = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        m_GlobalSettings->m_ForceUnits,
        m_Simulator->GetTotalImpulse());
    const double deliveredIsp = m_Simulator->GetTotalImpulse() / m_Motor->GetMotorPropellantMass(); //lb-sec / lb
    const double maxPressure = OpenBurnUnits::ConvertPressure(
        OpenBurnUnits::PressureUnits_T::psi,
        m_GlobalSettings->m_PressureUnits,
        m_Simulator->GetMaxPressure());
    const double maxMassFlux = m_Simulator->GetMaxMassFlux();

    const QString pressureUnits = OpenBurnUnits::GetPressureUnitSymbol(m_GlobalSettings->m_PressureUnits);
    const QString forceUnits = OpenBurnUnits::GetForceUnitSymbol(m_GlobalSettings->m_ForceUnits);

    m_lblMaxpressure->setText(QString::number(round(maxPressure)) + " "  + pressureUnits);
    m_lblCoreMassFlux->setText(QString::number(maxMassFlux, 'g', 3) + " " + massFluxUnits);
    m_lblBurnTime->setText(QString::number(m_Simulator->GetTotalBurnTime(), 'g', 3) + " s");
    m_lblTotalImpulse->setText(QString::number(round(totalImpulse)) +
                               " " + forceUnits + "-" + tr("sec"));
    m_lblIsp->setText(QString::number(deliveredIsp, 'f', 1) + tr(" s"));

    const QString designation(OpenBurnUtil::GetMotorClass(totalImpulseN));
    const QString thrust(QString::number(round(avgThrustN)));
    const QString percent(QString::number(OpenBurnUtil::GetMotorClassPercent(totalImpulseN), 'g', 2));
    m_lblMotorDesignation->setText(percent + "% " + designation + "-" + thrust);
}
void SimulationTab::OnDesignReady()
{
    if (m_GlobalSettings->m_redrawOnChanges)
    {
        UpdateSimulation();
    }
    m_btnRunSim->setEnabled(true);
}
void SimulationTab::OnSimSettingsButtonClicked()
{
    if (m_SimSettingsDialog == nullptr)
    {
        m_SimSettingsDialog = new SimSettingsDialog(m_SimSettings);
    }
    m_SimSettingsDialog->show();
    m_SimSettingsDialog->activateWindow();
    m_SimSettingsDialog->raise();
}
void SimulationTab::OnRunSimButtonClicked()
{
    UpdateSimulation();
}
void SimulationTab::OnSimSettingsChanged()
{
    UpdateSimulation();
}
void SimulationTab::SetGraphNames()
{
    const QString pressureUnits = OpenBurnUnits::GetPressureUnitSymbol(m_GlobalSettings->m_PressureUnits);
    m_Plotter->graph(0)->setName(tr("Chamber Pressure") + "(" + pressureUnits + ")");
    m_Plotter->graph(1)->setName(tr("Core Mass Flux") + "(" + massFluxUnits + ")");
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
void SimulationTab::UpdatePlotter()
{
    if (m_Simulator->GetResults().empty())
    {
        return;
    }
    //prepare graphs
    const int numPoints = m_Simulator->GetTotalBurnTime() / m_SimSettings->timeStep;
    const double maxPressure = OpenBurnUnits::ConvertPressure(
        OpenBurnUnits::PressureUnits_T::psi,
        m_GlobalSettings->m_PressureUnits,
        m_Simulator->GetMaxPressure());
    const double maxMassFlux = m_Simulator->GetMaxMassFlux();

    QVector<double> time(numPoints), pressure(numPoints), massflux(numPoints);
    for (int i=0; i<numPoints; ++i)
    {
        time[i] = m_Simulator->GetResults()[i]->time;
        pressure[i] = OpenBurnUnits::ConvertPressure(
            OpenBurnUnits::PressureUnits_T::psi,
            m_GlobalSettings->m_PressureUnits,
            m_Simulator->GetResults()[i]->pressure);
    }
    m_Plotter->graph(0)->setData(time, pressure);
    m_Plotter->xAxis->setRange(0.f, m_Simulator->GetTotalBurnTime() + .2f);
    m_Plotter->yAxis->setRange(0, 1.10* maxPressure);

    const double scaleFactor = 1.10 * maxPressure / maxMassFlux;
    for (int i = 0; i<numPoints; ++i)
    {
        massflux[i] = scaleFactor * m_Simulator->GetResults()[i]->massflux;
    }
    m_Plotter->graph(1)->setData(time, massflux);
    QCPAxis* massFluxAxis = m_Plotter->yAxis->axisRect()->axis(QCPAxis::atRight, 1);
    massFluxAxis->setRange(0.0f, maxMassFlux);

    m_Plotter->replot();
    SetGraphNames();
}
