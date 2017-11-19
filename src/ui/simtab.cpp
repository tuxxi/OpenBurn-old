#include <QGroupBox>
#include <QGridLayout>

#include "simtab.h"
#include "src/units.h"

SimulationTab::SimulationTab(OpenBurnMotor* motor, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent)
    : QWidget(parent), m_Motor(motor), m_Simulator(sim), m_SimSettingsDialog(nullptr), m_GlobalSettings(settings)
{
    SetupUI();
    connect(m_Motor, SIGNAL(DesignReady()), this, SLOT(OnDesignReady()));
    connect(m_btnRunSim, SIGNAL(clicked()), this, SLOT(OnRunSimButtonClicked()));
    connect(m_btnSimSettings, SIGNAL(clicked()), this, SLOT(OnSimSettingsButtonClicked()));
    m_SimSettings = new MotorSimSettings;
}
SimulationTab::~SimulationTab()
{
    delete m_SimSettings;
}
void SimulationTab::SetupUI()
{
    m_Plotter = new QCustomPlot;  
    m_Plotter->addGraph();
    // give the axes some labels:
    m_Plotter->xAxis->setLabel("Time");
    m_Plotter->yAxis->setLabel("Pressure");
    // set axes ranges, so we see all data:
    m_Plotter->xAxis->setRange(-.2f, 2.f);
    m_Plotter->yAxis->setRange(0.f, 1200.f);
    
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
    resultsLayout->addWidget(new QLabel(tr("Burn Time:")), 1, 0);
    resultsLayout->addWidget(m_lblBurnTime = new QLabel, 1, 1);
    resultsLayout->addWidget(new QLabel(tr("Total Impulse:")), 2, 0);
    resultsLayout->addWidget(m_lblTotalImpulse = new QLabel, 2, 1);
    resultsLayout->addWidget(new QLabel(tr("Motor Designation:")), 3, 0);
    resultsLayout->addWidget(m_lblMotorDesignation = new QLabel, 3, 1);
    
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
    if (m_Simulator->GetResults().empty())
    {
        return;
    }
    const int numPoints = m_Simulator->GetTotalBurnTime() / m_SimSettings->timeStep;
    double maxPressure = OpenBurnUnits::ConvertPressure(
        OpenBurnUnits::PressureUnits_T::psi,
        m_GlobalSettings->m_PressureUnits,
        m_Simulator->GetMaxPressure());

    QVector<double> x(numPoints), y(numPoints);
    for (int i=0; i<numPoints; ++i)
    {
        x[i] = m_Simulator->GetResults()[i]->time;
        y[i] = OpenBurnUnits::ConvertPressure(
            OpenBurnUnits::PressureUnits_T::psi,
            m_GlobalSettings->m_PressureUnits,
            m_Simulator->GetResults()[i]->pressure);
    }
    m_Plotter->graph(0)->setData(x,y);
    m_Plotter->xAxis->setRange(0.f, m_Simulator->GetTotalBurnTime() + .2f);
    m_Plotter->yAxis->setRange(0, maxPressure);
    m_Plotter->replot();

    //set results labels
    double totalImpulseN = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        m_Simulator->GetTotalImpulse());
    double avgThrustN = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        OpenBurnUnits::ForceUnits_T::newtons,
        m_Simulator->GetAvgThrust());

    /*
    double avgThrust = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        m_GlobalSettings->m_ForceUnits,
        m_Simulator->GetAvgThrust());
    */
    double totalImpulse = OpenBurnUnits::ConvertForce(
        OpenBurnUnits::ForceUnits_T::pounds_force,
        m_GlobalSettings->m_ForceUnits,
        m_Simulator->GetTotalImpulse());

    m_lblMaxpressure->setText(QString::number(round(maxPressure)) +
        " " +
        OpenBurnUnits::GetPressureUnitSymbol(m_GlobalSettings->m_PressureUnits));

    m_lblBurnTime->setText(QString::number(m_Simulator->GetTotalBurnTime(), 'g', 3) + " s");
    m_lblTotalImpulse->setText(QString::number(round(totalImpulse)) +
        " " +
        OpenBurnUnits::GetForceUnitSymbol(m_GlobalSettings->m_ForceUnits) +
        "-"+ tr("sec"));


    QString designation(OpenBurnUtil::GetMotorClass(totalImpulseN));
    QString thrust(QString::number(round(avgThrustN)));
    QString percent(QString::number(OpenBurnUtil::GetMotorClassPercent(totalImpulseN), 'g', 2));
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
