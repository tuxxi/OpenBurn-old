#pragma once
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QPropertyAnimation>
#include <QToolButton>

#include <memory>

#include "src/motorsim.hpp"
#include "src/settings.hpp"
#include "src/thirdparty/qcustomplot/qcustomplot.h"
#include "src/ui/dialogs/simsettingsdialog.hpp"
#include "src/ui/graphics/motorgraphicsitem.hpp"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~SimulationTab();

    void UpdateSimulation();
    void UpdateResults();
    void UpdateGraphics(OpenBurnMotor* motor = nullptr);
    OpenBurnMotor* GetCurrentSliceMotor();
    void resizeEvent(QResizeEvent* event) override;
private slots:
    void OnRunSimButtonClicked();
    void OnSimSettingsButtonClicked();
    void OnPlayAnimationButtonClicked();
    void OnToBeginningButtonClicked();
    void OnAnimationFinished();
    void OnDesignReady();
    void OnDesignUpdated();
    void OnXPosClicked(double newXPos);

    void OnSimSettingsChanged();
    void OnMotorSliceChanged(int sliceIndex); //scrubber bar
    void OnXPosChanged(int xPos);
private:
    void SetupUI();
    void SetGraphNames();
    void SetupPlotter();
    void UpdatePlotter();
    void UpdatePlotterLine();
    void UpdateCurrentChamber(int currentSlice);
    void UpdateCurrentXPos(double xpos);
    //gfx
    std::unique_ptr<MotorGraphicsItem> m_gfxMotor;
    QGraphicsView* m_MotorDisplayView;
    QGraphicsScene* m_MotorDisplayScene;
    double m_currentXPos;

    QSlider* m_sldBurnTimeScrubBar, *m_sldCurrentXPos;
    std::unique_ptr<QPropertyAnimation> m_animation;
    QLabel* m_lblMaxpressure, *m_lblBurnTime, *m_lblTotalImpulse, *m_lblMotorDesignation, *m_lblCoreMassFlux, *m_lblIsp;
    QLabel* m_lblCurrentPressure, *m_lblCurrentCoreMassFlux, *m_lblCurrentThrust, *m_lblCurrentIsp;
    QLabel* m_lblXPos, *m_lblXPosMassFlux, *m_lblXPosMachNumber;
    QPushButton* m_btnRunSim, *m_btnSimSettings, *m_btnPlayAnimation;
    QToolButton* m_btntToBeginning;

    QCustomPlot* m_Plotter;
    QCPItemLine* m_lineGraphTime;

    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
	OpenBurnSettings* m_GlobalSettings;
    std::unique_ptr<MotorSimSettings> m_SimSettings;
	std::unique_ptr<SimSettingsDialog> m_SimSettingsDialog;
};
