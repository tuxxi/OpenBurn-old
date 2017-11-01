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

#include "src/ui/widgets/graintablewidget.h"
#include "src/ui/dialogs/graindialog.h"
#include "src/ui/designtab.h"

#include "src/motorsim.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void SLOT_NewGrain(OpenBurnGrain *grain);

private slots:
    void menuQuit();
    void menuOpen();
    void menuSave();
    void menuSaveAs();
private:
    void closeEvent(QCloseEvent*);
    void SetupUI();

    QWidget *centralWidget;

    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QMenu *menuFile, *menuEdit, *menuTools, *menuHelp;

    QAction *actionOpen, *actionSave, *actionSave_As, *actionQuit, *actionNew, *actionExport;

    QTabWidget *tabWidget;
    DesignTab *m_designTab;    
    MotorSim *sim;
};
