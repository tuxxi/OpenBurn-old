#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <memory>

#include "src/motor.hpp"
#include "src/settings.hpp"
#include "src/application.hpp"

#include "src/ui/widgets/graintablewidget.hpp"
#include "src/ui/dialogs/graindialog.hpp"
#include "src/ui/designtab.hpp"
#include "src/ui/simtab.hpp"
#include "src/ui/propellanttab.hpp"
#include "src/ui/dialogs/globalsettingsdialog.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(OpenBurnApplication& app, QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent*) override;
    virtual void resizeEvent(QResizeEvent*) override;
private slots:
    //file
    void OnMenuNew();
    void OnMenuOpen();
    void OnMenuSave();
    void OnMenuSaveAs();
    void OnMenuEngExport();
    void OnMenuQuit();

    //tools
    void OnMenuSettings();

    //edit
    void OnMenuDelete();

    //simulator
    void OnSimulationStarted();
    void OnSimulationFinished(bool success);

    //databases
    void OnPropellantsUpdated();
    void OnSettingsChanged();

    //usability
    void OnTabChanged(int index);
    void OnSelectionChanged(bool isSelected);
private:
    void SetupUI();

    OpenBurnApplication& m_app;
    QWidget *m_CentralWidget;

    QMenuBar *m_MenuBar;
    QMenu *m_MenuFile, *m_MenuEdit, *m_MenuTools, *m_MenuHelp;
    //file
    QMenu *m_MenuExport;
    QAction *m_ActionOpen, *m_ActionSave, *m_ActionSaveAs, *m_ActionQuit, *m_ActionNew, *m_ActionEngExport;
    //tools
    QAction *m_ActionSettings;
	//edit
	QAction *m_ActionUndo, *m_ActionRedo, *m_ActionDelete;

    QStatusBar *m_StatusBar;

    QTabWidget *m_TabWidget;
	DesignTab* m_DesignTab;
	SimulationTab* m_SimTab;
	PropellantTab* m_PropellantTab;

};
