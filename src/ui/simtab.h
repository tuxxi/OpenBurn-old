#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "src/motorsim.h"
#include "src/settings.h"
#include "src/thirdparty/qcustomplot/qcustomplot.h"
#include "src/ui/dialogs/simsettingsdialog.h"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~SimulationTab();

    void UpdateSimulation();
    void UpdateResults();
private slots:
    void OnRunSimButtonClicked();
    void OnSimSettingsButtonClicked();
    void OnDesignReady();
    void OnSimSettingsChanged();
private:
    void SetupUI();
    void SetGraphNames();
    void SetupPlotter();
    void UpdatePlotter();

    QLabel* m_lblMaxpressure, *m_lblBurnTime, *m_lblTotalImpulse, *m_lblMotorDesignation, *m_lblCoreMassFlux, *m_lblIsp;
    QCustomPlot* m_Plotter;
    QPushButton* m_btnRunSim, *m_btnSimSettings;
    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
    MotorSimSettings* m_SimSettings;
    OpenBurnSettings* m_GlobalSettings;
    SimSettingsDialog* m_SimSettingsDialog;
};
