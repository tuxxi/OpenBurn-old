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
#include "src/ui/dialogs/globalsettingsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent*) override;
    virtual void resizeEvent(QResizeEvent*) override;
private slots:
    void OnMenuQuit();
    void OnMenuOpen();
    void OnMenuSave();
    void OnMenuSaveAs();
    void OnMenuNew();
    void OnMenuSettings();

    void OnSimulationStarted();
    void OnSimulationFinished(bool success);
    void OnPropellantsUpdated();
    void OnSettingsChanged();
    void OnTabChanged(int index);
private:
    void SetupUI();
    void SaveFile(QString filename);

    bool LoadSettings(const QString& filename);
    bool SaveSettings();

    OpenBurnMotor* m_DesignMotor;
    PropellantList* m_Propellants;
    MotorSim* m_Simulator;
    OpenBurnSettings* m_GlobalSettings;
    QString m_CurrentDesignFilename;

    QString m_SettingsFileName;
    
    QWidget *m_CentralWidget;

    QMenuBar *m_MenuBar;
    QMenu *m_MenuFile, *m_MenuEdit, *m_MenuTools, *m_MenuHelp;
    //file
    QAction *m_ActionOpen, *m_ActionSave, *m_ActionSaveAs, *m_ActionQuit, *m_ActionNew, *m_ActionExport;
    //tools
    QAction *m_ActionSettings;

    QStatusBar *m_StatusBar;

    QTabWidget *m_TabWidget;
    DesignTab *m_DesignTab;
    SimulationTab* m_SimTab;
    PropellantTab* m_PropellantTab;

};
