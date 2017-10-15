#pragma once

#include <QMainWindow>
#include <QSizePolicy>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QVBoxLayout>

#include "src/ui/graintablewidget.h"
#include "src/ui/graindialog.h"
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
