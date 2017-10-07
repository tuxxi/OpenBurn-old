#pragma once

#include <QMainWindow>
#include "graindialog.h"

#include "src/motorsim.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void DialogClosed();
    void NewGrain(OpenBurnGrain *grain);
    void GrainPositionUpdated(int &oldPosition, int &newPosition);

private slots:
    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_triggered();
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    void closeEvent(QCloseEvent*);

    Ui::MainWindow *ui;
    GrainDialog *grainDialog;

    MotorSim *sim;
};
