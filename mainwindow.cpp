#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_openburn.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("OpenBurn File (*.obm);;BurnSim File (*.bsx)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        file.close();
    }

}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("OpenBurn File (*.obm);;BurnSim File (*.bsx)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
            return;
        }
        else
        {
            /*QTextStream stream(&file);
            stream << ui->textEdit->toPlainText();
            stream.flush();
            */
            file.close();
        }
    }

}

void MainWindow::on_actionSave_triggered()
{
    //save file
}

void MainWindow::on_pushButton_clicked()
{
    dialog = new GrainDialog();
    dialog->show();
}
