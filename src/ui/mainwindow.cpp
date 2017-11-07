#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_DesignMotor = new OpenBurnMotor();
    m_Propellants = new PropellantList();
    m_Simulator = new MotorSim(m_DesignMotor);
    SetupUI();
    connect(m_Simulator, SIGNAL(SimulationStarted()), this, SLOT(SLOT_SimulationStarted()));
    connect(m_Simulator, SIGNAL(SimulationFinished(bool)), this, SLOT(SLOT_SimulationFinished(bool))); 
    connect(m_PropellantTab, SIGNAL(PropellantsUpdated()), this, SLOT(SLOT_PropellantsUpdated()));
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
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    
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

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
    
    tabWidget = new QTabWidget(this);
    m_designTab = new DesignTab(m_DesignMotor, m_Propellants);
    m_SimTab = new SimulationTab(m_DesignMotor, m_Simulator);
    m_PropellantTab = new PropellantTab(m_Propellants);
    tabWidget->addTab(m_designTab, tr("Design"));
    tabWidget->addTab(m_SimTab, tr("Simulation"));
    tabWidget->addTab(m_PropellantTab, tr("Propellants"));
    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete m_DesignMotor;
    delete m_Propellants;
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
            tr("OpenBurn File (*.obm)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        QJsonObject motor = loadDoc.object();
        m_DesignMotor->ReadJSON(motor, m_Propellants);
        file.close();
    }
    m_CurrentFilename = fileName;
    m_designTab->UpdateDesign();
    m_statusBar->showMessage(tr("Opened file ") + m_CurrentFilename, 3000);    
}

void MainWindow::menuSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("OpenBurn File (*.obm)"));
    SaveFile(fileName);
}
void MainWindow::menuSave()
{
    if (!m_CurrentFilename.isEmpty())
    {
        SaveFile(m_CurrentFilename);
    }
    else
    {
        menuSaveAs();
    }
}
void MainWindow::SaveFile(QString fileName)
{
    if (!fileName.isEmpty())
    {
        if (!fileName.contains(".obm"))
        {
            fileName += ".obm";
        }
        m_CurrentFilename = fileName;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
            return;
        }
        else
        {
            QJsonObject motorObject;
            m_DesignMotor->WriteJSON(motorObject);
            QJsonDocument saveDoc(motorObject);
            file.write(saveDoc.toJson());
            file.close();
            m_statusBar->showMessage(tr("File saved."), 3000);
        }
    }

}
void MainWindow::SLOT_SimulationStarted()
{
    m_statusBar->showMessage(tr("Starting simulation"), 3000);    
}
void MainWindow::SLOT_SimulationFinished(bool success)
{
    if (success)
    {
        m_statusBar->showMessage(tr("Simulation finished"), 5000);            
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Simulation ERROR"));        
        m_statusBar->showMessage(tr("Simulation ERROR!"), 5000);            
    }
}
//this slot sets all the grains to a new propellant type if the propellant database happens to change
void MainWindow::SLOT_PropellantsUpdated()
{
    for (auto prop : *m_Propellants)
    {
        for (auto grain : m_DesignMotor->GetGrains())
        {
            if (grain->GetPropellantType().GetPropellantName() == prop.GetPropellantName())
            {
                grain->SetPropellantType(prop);
            }
        }    
    }
}