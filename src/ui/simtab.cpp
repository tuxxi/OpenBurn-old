#include <QGroupBox>
#include <QGridLayout>

#include "simtab.h"

SimulationTab::SimulationTab(OpenBurnMotor* motor, QWidget* parent)
    : QWidget(parent), m_Motor(motor)
{
    SetupUI();
    m_Simulator = new MotorSim(m_Motor);  
    connect(m_RunSimulationButton, SIGNAL(clicked()), this, SLOT(RunSimButton_Clicked())); 
    connect(m_Motor, SIGNAL(SIG_DesignReady()), this, SLOT(SLOT_DesignReady())); 
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
    
    m_RunSimulationButton = new QPushButton(tr("Run Sim"));
    m_RunSimulationButton->setEnabled(false);
    QGroupBox* gb_Simulate = new QGroupBox(tr("Motor Simulation"));
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(m_Plotter, 0, 0, 2, 4);
    gridLayout->addWidget(m_RunSimulationButton, 0, 3, 1, 1);
    gb_Simulate->setLayout(gridLayout);
    
    QVBoxLayout* masterLayout = new QVBoxLayout;
    masterLayout->addWidget(gb_Simulate);
    setLayout(masterLayout);
}
SimulationTab::~SimulationTab()
{

}
void SimulationTab::RunSimButton_Clicked()
{
    double timestep = 0.01f;
    m_Simulator->RunSim(timestep);
    const int numPoints = m_Simulator->GetTotalBurnTime() / timestep;

    double maxPressure = 0, minPressure = m_Simulator->GetResults()[0]->pressure;;
    QVector<double> x(numPoints), y(numPoints);
    for (int i=0; i<numPoints; ++i)
    {
        x[i] = m_Simulator->GetResults()[i]->time;
        y[i] = m_Simulator->GetResults()[i]->pressure;
        if (y[i] > maxPressure)
        {
            maxPressure = y[i];
        }
        if (y[i] < minPressure)
        {
            minPressure = y[i];
        }
    }
    m_Plotter->graph(0)->setData(x,y);
    m_Plotter->xAxis->setRange(0.f, m_Simulator->GetTotalBurnTime() + .2f);
    m_Plotter->yAxis->setRange(minPressure, maxPressure);
    m_Plotter->replot();
}
void SimulationTab::SLOT_DesignReady()
{
    m_RunSimulationButton->setEnabled(true);
}