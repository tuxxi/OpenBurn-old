#include <QWidget>
#include <QPushButton>

#include "src/motorsim.h"
#include "src/ui/widgets/qcustomplot.h"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, QWidget* parent = nullptr);
    ~SimulationTab();
private slots:
    void RunSimButton_Clicked();
    void SLOT_DesignReady();
private:
    void SetupUI();

    QCustomPlot* m_Plotter;
    QPushButton* m_RunSimulationButton;
    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
};