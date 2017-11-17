#pragma once

#include <QMainWindow>
#include <QSizePolicy>
#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QStatusBar>

#include "src/motor.h"
#include "src/settings.h"

#include "src/ui/widgets/graintablewidget.h"
#include "src/ui/dialogs/graindialog.h"
#include "src/ui/designtab.h"
#include "src/ui/simtab.h"
#include "src/ui/propellanttab.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void menuQuit();
    void menuOpen();
    void menuSave();
    void menuSaveAs();

    void SLOT_SimulationStarted();
    void SLOT_SimulationFinished(bool success);
    void SLOT_PropellantsUpdated();
private:
    void closeEvent(QCloseEvent*);
    void SetupUI();
    void SaveFile(QString filename);
    QWidget *centralWidget;

    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QMenu *menuFile, *menuEdit, *menuTools, *menuHelp;

    QAction *actionOpen, *actionSave, *actionSave_As, *actionQuit, *actionNew, *actionExport;

    QStatusBar *m_statusBar;
    QTabWidget *tabWidget;
    DesignTab *m_designTab;
    SimulationTab* m_SimTab;
    PropellantTab* m_PropellantTab;

    OpenBurnMotor* m_DesignMotor;
    PropellantList* m_Propellants;
    MotorSim* m_Simulator;
    OpenBurnSettings* m_GlobalSettings;
    
    QString m_CurrentFilename;
};
