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
    m_GlobalSettings = new OpenBurnSettings();
    SetupUI();
    connect(m_Simulator, &MotorSim::SimulationStarted,
            this, &MainWindow::OnSimulationStarted);
    connect(m_Simulator, &MotorSim::SimulationFinished,
            this, &MainWindow::OnSimulationFinished);
    connect(m_PropellantTab, &PropellantTab::PropellantsUpdated,
            this, &MainWindow::OnPropellantsUpdated);
    connect(m_GlobalSettings, &OpenBurnSettings::SettingsChanged,
            this, &MainWindow::OnSettingsChanged);
}
MainWindow::~MainWindow()
{
    delete m_DesignMotor;
    delete m_Propellants;
    delete m_Simulator;
    delete m_GlobalSettings;
}
void MainWindow::SetupUI()
{
    if (objectName().isEmpty())
        setObjectName(QStringLiteral("MainWindow"));

    setWindowTitle(tr("OpenBurn"));
    setGeometry(100, 100, 800, 600);
    m_MenuBar = new QMenuBar(this);
    m_MenuBar->setGeometry(QRect(0, 0, 800, 20));
    setMenuBar(m_MenuBar);
    m_StatusBar = new QStatusBar;
    setStatusBar(m_StatusBar);
    
    m_MenuFile = new QMenu(m_MenuBar);
    m_MenuFile->setTitle(tr("File"));
    m_MenuBar->addAction(m_MenuFile->menuAction());

    m_ActionNew = new QAction(this);
    m_ActionNew->setText(tr("New"));
    m_ActionNew->setShortcuts(QKeySequence::New);    
    connect(m_ActionNew, &QAction::triggered, this, &MainWindow::OnMenuNew);
    m_MenuFile->addAction(m_ActionNew);

    m_ActionOpen = new QAction(this);
    m_ActionOpen->setText(tr("Open"));
    m_ActionOpen->setShortcuts(QKeySequence::Open);    
    connect(m_ActionOpen, &QAction::triggered, this, &MainWindow::OnMenuOpen);
    m_MenuFile->addAction(m_ActionOpen);

    m_ActionSave = new QAction(this);
    m_ActionSave->setText(tr("Save"));
    m_ActionSave->setShortcuts(QKeySequence::Save);
    connect(m_ActionSave, &QAction::triggered, this, &MainWindow::OnMenuSave);
    m_MenuFile->addAction(m_ActionSave);

    m_ActionSaveAs = new QAction(this);
    m_ActionSaveAs->setText(tr("Save As..."));
    m_ActionSaveAs->setShortcuts(QKeySequence::SaveAs);
    connect(m_ActionSaveAs, &QAction::triggered, this, &MainWindow::OnMenuSaveAs);
    m_MenuFile->addAction(m_ActionSaveAs);

    m_ActionExport = new QAction(this);
    m_ActionExport->setText(tr("Export"));    
    //connect(actionExport, &QAction::triggered, this, &MainWindow::menuExport);
    m_MenuFile->addAction(m_ActionExport);

    m_MenuFile->addSeparator();

    m_ActionQuit = new QAction(this);
    m_ActionQuit->setText(tr("Quit"));
    m_ActionQuit->setShortcuts(QKeySequence::Quit);
    connect(m_ActionQuit, &QAction::triggered, this, &MainWindow::OnMenuQuit);
    m_MenuFile->addAction(m_ActionQuit);



    m_MenuEdit = new QMenu(m_MenuBar);
    m_MenuEdit->setTitle(tr("Edit"));
    m_MenuBar->addAction(m_MenuEdit->menuAction());

    m_MenuTools = new QMenu(m_MenuBar);
    m_MenuTools->setTitle(tr("Tools"));    
    m_MenuBar->addAction(m_MenuTools->menuAction());

    m_ActionSettings = new QAction(this);
    m_ActionSettings->setText(tr("Settings"));
    m_ActionSettings->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(m_ActionSettings, &QAction::triggered, this, &MainWindow::OnMenuSettings);
    m_MenuTools->addAction(m_ActionSettings);

    m_MenuHelp = new QMenu(m_MenuBar);
    m_MenuHelp->setTitle(tr("Help"));
    m_MenuBar->addAction(m_MenuHelp->menuAction());

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
    
    m_TabWidget = new QTabWidget(this);
    m_DesignTab = new DesignTab(m_DesignMotor, m_Propellants, m_GlobalSettings);
    m_SimTab = new SimulationTab(m_DesignMotor, m_Simulator, m_GlobalSettings);
    m_PropellantTab = new PropellantTab(m_Propellants);
    m_TabWidget->addTab(m_DesignTab, tr("Design"));
    m_TabWidget->addTab(m_SimTab, tr("Simulation"));
    m_TabWidget->addTab(m_PropellantTab, tr("Propellants"));
    setCentralWidget(m_TabWidget);
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
void MainWindow::OnMenuNew()
{
    if (m_DesignMotor)
    {
        delete m_DesignMotor;
    }
    if (m_DesignTab)
    {
        delete m_DesignTab;
    }
    m_DesignMotor = new OpenBurnMotor;
    m_DesignTab = new DesignTab(m_DesignMotor, m_Propellants, m_GlobalSettings);
    m_TabWidget->insertTab(0, m_DesignTab, tr("Design"));
    m_TabWidget->setCurrentIndex(0);
}
void MainWindow::OnMenuOpen()
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
    m_CurrentDesignFilename = fileName;
    m_DesignTab->UpdateDesign();
    m_StatusBar->showMessage(tr("Opened file ") + m_CurrentDesignFilename, 3000);
}
void MainWindow::OnMenuSave()
{
    if (!m_CurrentDesignFilename.isEmpty())
    {
        SaveFile(m_CurrentDesignFilename);
    }
    else
    {
        OnMenuSaveAs();
    }
}
void MainWindow::OnMenuSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("OpenBurn File (*.obm)"));
    SaveFile(fileName);
}
void MainWindow::OnMenuQuit()
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
void MainWindow::OnMenuSettings()
{
    //we don't have to worry about memory managment because of WA_DeleteOnClose
    GlobalSettingsDialog* dialog = new GlobalSettingsDialog(m_GlobalSettings);
    dialog->activateWindow();
    dialog->show();
    dialog->raise();
}
void MainWindow::SaveFile(QString fileName)
{
    if (!fileName.isEmpty())
    {
        if (!fileName.contains(".obm"))
        {
            fileName += ".obm";
        }
        m_CurrentDesignFilename = fileName;
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
            m_StatusBar->showMessage(tr("File saved."), 3000);
        }
    }

}
void MainWindow::OnSimulationStarted()
{
    m_StatusBar->showMessage(tr("Starting simulation"), 3000);
}
void MainWindow::OnSimulationFinished(bool success)
{
    if (success)
    {
        m_StatusBar->showMessage(tr("Simulation finished"), 5000);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Simulation ERROR"));        
        m_StatusBar->showMessage(tr("Simulation ERROR!"), 5000);
    }
}
//this slot sets all the grains to a new propellant type if the propellant database happens to change
void MainWindow::OnPropellantsUpdated()
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
void MainWindow::OnSettingsChanged()
{
    m_StatusBar->showMessage(tr("Settings updated."), 5000);

    emit m_DesignMotor->DesignUpdated();
    m_SimTab->UpdateResults();
}
