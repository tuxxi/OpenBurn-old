#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "src/motorsim.h"
#include "src/ui/widgets/qcustomplot.h"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, MotorSim* sim, QWidget* parent = nullptr);
    ~SimulationTab();
private slots:
    void RunSimButton_Clicked();
    void SLOT_DesignReady();
    void UpdateSimulation();    
private:
    void SetupUI();

    QLabel* m_maxPressureLabel, *m_BurnTimeLabel, *m_totalImpulseLabel, *m_motorDesignationLabel;
    QCustomPlot* m_Plotter;
    QPushButton* m_RunSimulationButton, *m_SimSettingsButton;
    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
};