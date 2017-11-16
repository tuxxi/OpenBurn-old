#include <QGroupBox>
#include <QGridLayout>

#include "simtab.h"
#include "src/units.h"

SimulationTab::SimulationTab(OpenBurnMotor* motor, MotorSim* sim, QWidget* parent)
    : QWidget(parent), m_Motor(motor), m_Simulator(sim), m_SettingsDialog(nullptr)
{
    SetupUI();
    connect(m_Motor, SIGNAL(SIG_DesignReady()), this, SLOT(SLOT_DesignReady()));
    connect(m_RunSimulationButton, SIGNAL(clicked()), this, SLOT(RunSimButton_Clicked())); 
    connect(m_SimSettingsButton, SIGNAL(clicked()), this, SLOT(SimSettingsButton_Clicked()));
    m_Settings = new MotorSimSettings;
}
SimulationTab::~SimulationTab()
{
    delete m_Settings;
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
    
    m_RunSimulationButton = new QPushButton(tr("Run Simulation"));
    m_RunSimulationButton->setEnabled(false);    
    m_SimSettingsButton = new QPushButton(tr("Simulation Settings"));

    QGroupBox* gb_Simulate = new QGroupBox(tr("Motor Simulation"));
    QGroupBox* gb_Controls = new QGroupBox(tr("Controls"));    
    QGroupBox* gb_Results = new QGroupBox(tr("Results"));

    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addWidget(m_RunSimulationButton);
    controlsLayout->addWidget(m_SimSettingsButton);
    gb_Controls->setLayout(controlsLayout);

    QGridLayout* resultsLayout = new QGridLayout;
    resultsLayout->addWidget(new QLabel(tr("Max Pc: (psi)")), 0, 0);
    resultsLayout->addWidget(m_maxPressureLabel = new QLabel, 0, 1);
    resultsLayout->addWidget(new QLabel(tr("Burn Time: (s)")), 1, 0);
    resultsLayout->addWidget(m_BurnTimeLabel = new QLabel, 1, 1);
    resultsLayout->addWidget(new QLabel(tr("Total Impulse: (N*sec)")), 2, 0);
    resultsLayout->addWidget(m_totalImpulseLabel = new QLabel, 2, 1);
    resultsLayout->addWidget(new QLabel(tr("Motor Designation:")), 3, 0);
    resultsLayout->addWidget(m_motorDesignationLabel = new QLabel, 3, 1);
    
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
    m_Simulator->RunSim(m_Settings);
    const int numPoints = m_Simulator->GetTotalBurnTime() / m_Settings->timeStep;

    double maxPressure = m_Simulator->GetMaxPressure();
    QVector<double> x(numPoints), y(numPoints);
    for (int i=0; i<numPoints; ++i)
    {
        x[i] = m_Simulator->GetResults()[i]->time;
        y[i] = m_Simulator->GetResults()[i]->pressure;
    }
    m_Plotter->graph(0)->setData(x,y);
    m_Plotter->xAxis->setRange(0.f, m_Simulator->GetTotalBurnTime() + .2f);
    m_Plotter->yAxis->setRange(0, maxPressure);
    m_Plotter->replot();

    //set results labels
    double nsec = OpenBurnUnits::PoundsToNewtons(m_Simulator->GetTotalImpulse());
    
    m_maxPressureLabel->setText(QString::number(round(maxPressure)));
    m_BurnTimeLabel->setText(QString::number(m_Simulator->GetTotalBurnTime(), 'g', 3));
    m_totalImpulseLabel->setText(QString::number(round(nsec)));

    double thrustN = OpenBurnUnits::PoundsToNewtons(m_Simulator->GetAvgThrust());
    QString designation(OpenBurnUtil::GetMotorClass(nsec));
    QString thrust(QString::number(round(thrustN)));
    QString percent(QString::number(OpenBurnUtil::GetMotorClassPercent(nsec), 'g', 2));
    m_motorDesignationLabel->setText(percent + "% " + designation + "-" + thrust);
}
void SimulationTab::SLOT_DesignReady()
{
    m_RunSimulationButton->setEnabled(true);
}
void SimulationTab::SimSettingsButton_Clicked()
{
    if (m_SettingsDialog == nullptr)
    {
        m_SettingsDialog = new SimSettingsDialog(m_Settings);
    }
    m_SettingsDialog->show();
    m_SettingsDialog->activateWindow();
    m_SettingsDialog->raise();
}
void SimulationTab::RunSimButton_Clicked()
{
    UpdateSimulation();
}