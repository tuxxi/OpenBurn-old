#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    SetupUI();
    connect(m_designTab, SIGNAL(SIG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_NewGrain(OpenBurnGrain*)));
    sim = new MotorSim();    
}
void MainWindow::SetupUI()
{
    if (objectName().isEmpty())
        setObjectName(QStringLiteral("MainWindow"));

    setWindowTitle(tr("OpenBurn"));
    setGeometry(100, 100, 800, 600);
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 800, 20));
    setMenuBar(menuBar);
    
    menuFile = new QMenu(menuBar);
    menuEdit = new QMenu(menuBar);
    menuTools = new QMenu(menuBar);
    menuHelp = new QMenu(menuBar);
    
    menuFile->setTitle(tr("File"));
    menuEdit->setTitle(tr("Edit"));
    menuTools->setTitle(tr("Tools"));    
    menuHelp->setTitle(tr("Help"));

    actionNew = new QAction(this);
    actionNew->setText(tr("New"));
    actionNew->setShortcuts(QKeySequence::New);    
    //connect(actionNew, &QAction::triggered, this, &MainWindow::menuNew);
    
    actionOpen = new QAction(this);
    actionOpen->setText(tr("Open"));
    actionOpen->setShortcuts(QKeySequence::Open);    
    connect(actionOpen, &QAction::triggered, this, &MainWindow::menuOpen);

    actionSave = new QAction(this);
    actionSave->setText(tr("Save"));
    actionSave->setShortcuts(QKeySequence::Save);
    connect(actionSave, &QAction::triggered, this, &MainWindow::menuSave);

    actionSave_As = new QAction(this);
    actionSave_As->setText(tr("Save As..."));
    actionSave_As->setShortcuts(QKeySequence::SaveAs);
    connect(actionSave_As, &QAction::triggered, this, &MainWindow::menuSaveAs);
    
    actionExport = new QAction(this);
    actionExport->setText(tr("Export"));    
    //connect(actionExport, &QAction::triggered, this, &MainWindow::menuExport);

    actionQuit = new QAction(this);
    actionQuit->setText(tr("Quit"));
    actionQuit->setShortcuts(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &MainWindow::menuQuit);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuTools->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSave_As);
    menuFile->addAction(actionExport);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
    
    tabWidget = new QTabWidget(this);
    m_designTab = new DesignTab;
    tabWidget->addTab(m_designTab, tr("Design"));

    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete sim;
}
void MainWindow::SLOT_NewGrain(OpenBurnGrain *grain)
{
    //qDebug() << "new grain!";
    sim->AddGrain(grain);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn =
            QMessageBox::question( this, "OpenBurn", tr("Are you sure?\n"),
            QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
void MainWindow::menuQuit()
{
    QMessageBox::StandardButton resBtn =
    QMessageBox::question( this, "OpenBurn", tr("Are you sure?\n"),
        QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
        QCoreApplication::quit();
    }
    else
    {
        return;
    }
}
void MainWindow::menuOpen()
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

void MainWindow::menuSaveAs()
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
void MainWindow::menuSave()
{
    //save file
}
