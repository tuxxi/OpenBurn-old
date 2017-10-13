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
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTools;
    QMenu *menuHelp;

    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionQuit;
    QAction *actionNew;
    QAction *actionExport;

    QTabWidget *tabWidget;
    DesignTab *tab;
    QWidget *tab_2;
    QWidget *tab_3;
    
    MotorSim *sim;
};
