#pragma once

#include <QMainWindow>
#include "graindialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_triggered();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    GrainDialog *dialog;
};
