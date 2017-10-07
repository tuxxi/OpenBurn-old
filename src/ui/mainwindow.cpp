#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    grainDialog = nullptr;
    ui->setupUi(this);
    sim = new MotorSim();

    connect(ui->tableWidget, SIGNAL(GrainPositionUpdated(int&,int&)),
            this, SLOT(GrainPositionUpdated(int&,int&)) );

}

MainWindow::~MainWindow()
{
    delete ui;
    delete grainDialog;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn =
            QMessageBox::question( this, "OpenBurn", tr("Are you sure?\n"),
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
    if (grainDialog) grainDialog->close();
}
void MainWindow::DialogClosed()
{
    grainDialog = nullptr;
}
void MainWindow::NewGrain(OpenBurnGrain *grain)
{
    int insertRow = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(insertRow);
    ui->tableWidget->setItem(insertRow, 0, new QTableWidgetItem(QString::number(grain->GetLength())));
    ui->tableWidget->setItem(insertRow, 1, new QTableWidgetItem(QString::number(grain->GetDiameter())));
    ui->tableWidget->setItem(insertRow, 2, new QTableWidgetItem(QString::number(grain->GetCoreDiameter())));
    ui->tableWidget->setItem(insertRow, 3, new QTableWidgetItem(grain->GetPropellantType().GetPropellantName()));

    sim->AddGrain(grain);
}
void MainWindow::GrainPositionUpdated(int &oldPos, int &newPos)
{
    sim->SwapGrains(oldPos, newPos);
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
    if (grainDialog == nullptr) //only create one!
    {
        grainDialog = new GrainDialog();
        connect(grainDialog, SIGNAL(DialogClosed()),
                this, SLOT(DialogClosed()));
        connect(grainDialog, SIGNAL(NewGrain(OpenBurnGrain*)),
                this, SLOT(NewGrain(OpenBurnGrain*)));
    }
    grainDialog->show();
    grainDialog->activateWindow();
    grainDialog->raise();
}

void MainWindow::on_pushButton_5_clicked()
{
    if (grainDialog == nullptr)
    {
        grainDialog = new GrainDialog(nullptr, false);
        connect(grainDialog, SIGNAL(DialogClosed()),
                    this, SLOT(DialogClosed()));
        connect(grainDialog, SIGNAL(NewGrain(OpenBurnGrain*)),
                this, SLOT(NewGrain(OpenBurnGrain*)));

    }
    grainDialog->show();
    grainDialog->activateWindow();
    grainDialog->raise();
}
