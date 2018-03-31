#include <QFileDialog>

#include "mainwindow.hpp"
#include "src/export.hpp"
#include "src/ui/dialogs/exportdialog.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ImplicitIntegerAndEnumConversion"
MainWindow::MainWindow(OpenBurnApplication& app, QWidget *parent)
    : m_app(app), QMainWindow(parent)
{
    SetupUI();
}
MainWindow::~MainWindow()
{
}
void MainWindow::SetupUI()
{
    setWindowTitle(tr("OpenBurn"));
    setGeometry(100, 100, 800, 600);
    m_MenuBar = new QMenuBar(this);
    setMenuBar(m_MenuBar);
    m_StatusBar = new QStatusBar;
    setStatusBar(m_StatusBar);
    
    m_MenuFile = new QMenu(tr("File"), m_MenuBar);
    m_MenuBar->addAction(m_MenuFile->menuAction());

    m_ActionNew = new QAction(tr("New"), this);
    m_ActionNew->setShortcuts(QKeySequence::New);    
    connect(m_ActionNew, &QAction::triggered, this, &MainWindow::OnMenuNew);
    m_MenuFile->addAction(m_ActionNew);

    m_ActionOpen = new QAction(tr("Open"), this);
    m_ActionOpen->setShortcuts(QKeySequence::Open);    
    connect(m_ActionOpen, &QAction::triggered, this, &MainWindow::OnMenuOpen);
    m_MenuFile->addAction(m_ActionOpen);

    m_ActionSave = new QAction(tr("Save"), this);
    m_ActionSave->setShortcuts(QKeySequence::Save);
    connect(m_ActionSave, &QAction::triggered, this, &MainWindow::OnMenuSave);
    m_MenuFile->addAction(m_ActionSave);

    m_ActionSaveAs = new QAction(tr("Save As..."), this);
    m_ActionSaveAs->setShortcuts(QKeySequence::SaveAs);
    connect(m_ActionSaveAs, &QAction::triggered, this, &MainWindow::OnMenuSaveAs);
    m_MenuFile->addAction(m_ActionSaveAs);

    m_MenuExport = m_MenuFile->addMenu(tr("Export"));
    m_ActionEngExport = new QAction(tr("To .eng (RockSim, OpenRocket)"), this);
    m_MenuExport->addAction(m_ActionEngExport);
    connect(m_ActionEngExport, &QAction::triggered, this, &MainWindow::OnMenuEngExport);

    m_MenuFile->addSeparator();

    m_ActionQuit = new QAction(tr("Quit"), this);
    m_ActionQuit->setShortcuts(QKeySequence::Quit);
    connect(m_ActionQuit, &QAction::triggered, this, &MainWindow::OnMenuQuit);
    m_MenuFile->addAction(m_ActionQuit);


    m_MenuEdit = new QMenu(tr("Edit"), m_MenuBar);
    m_MenuBar->addAction(m_MenuEdit->menuAction());
	m_ActionUndo = m_app.GetUndoStack().createUndoAction(this, tr("&Undo"));
	m_ActionUndo->setShortcuts(QKeySequence::Undo);
	m_ActionRedo = m_app.GetUndoStack().createRedoAction(this, tr("&Redo"));
	m_ActionRedo->setShortcuts(QKeySequence::Redo);

    m_ActionDelete = new QAction(tr("&Delete"), this);
    m_ActionDelete->setShortcuts(QKeySequence::Delete);
    m_ActionDelete->setEnabled(false); //we start disabled because nothing is selected! the action will be re-enabled later
    connect(m_ActionDelete, &QAction::triggered, this, &MainWindow::OnMenuDelete);
	m_MenuEdit->addAction(m_ActionUndo);
	m_MenuEdit->addAction(m_ActionRedo);
    m_MenuEdit->addAction(m_ActionDelete);

    m_MenuTools = new QMenu(tr("Tools"), m_MenuBar);
    m_MenuBar->addAction(m_MenuTools->menuAction());

    m_ActionSettings = new QAction(tr("Settings"), this);
    m_ActionSettings->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(m_ActionSettings, &QAction::triggered, this, &MainWindow::OnMenuSettings);
    m_MenuTools->addAction(m_ActionSettings);

    m_MenuHelp = new QMenu(tr("Help"), m_MenuBar);
    m_MenuBar->addAction(m_MenuHelp->menuAction());

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
    
    m_TabWidget = new QTabWidget(this);
    connect(m_TabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::OnTabChanged);

    //m_DesignTab =
    //m_SimTab =
    //m_PropellantTab =

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
void MainWindow::resizeEvent(QResizeEvent * event)
{
    QApplication::sendEvent(m_DesignTab, event);
    QApplication::sendEvent(m_SimTab, event);
}
void MainWindow::OnMenuNew()
{
    //TODO: are you sure?
    m_app.ResetCurrentDesign();
}
void MainWindow::OnMenuOpen()
{
    //TODO: if (changes)
    m_app.ResetCurrentDesign();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("OpenBurn File (*.obm)"));

    m_StatusBar->showMessage(tr("Opened file ") + m_app.GetDesignFilename(), 3000);
}
void MainWindow::OnMenuSave()
{
    if (m_app.CurrentDesignSaved())
    {
        if (!m_app.SaveCurrentFile())
        {
            QMessageBox::critical(this,
                                  tr("Could not save file!"),
                                  tr("Could not save file."));
        }
    }
    else
    {
        OnMenuSaveAs();
    }
}
void MainWindow::OnMenuSaveAs()
{
    QFile file = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("OpenBurn File (*.obm)"));
    if (!m_app.SaveNewFile(file))
    {
        QMessageBox::critical(this,
                              tr("Could not save file!"),
                              tr("Could not save file."));

    }

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
    //we don't have to worry about memory management because of WA_DeleteOnClose
    auto dialog = new GlobalSettingsDialog(m_app);
    dialog->activateWindow();
    dialog->show();
    dialog->raise();
}

void MainWindow::OnMenuDelete()
{
    m_DesignTab->DeleteSelectedGrains();
}

void MainWindow::OnMenuEngExport()
{
    if (m_app.GetSimulator().GetResultsEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("No simulation results!"));
        return;
    }
    EngExport engExporter(m_app);
    //we don't have to worry about memory management because of WA_DeleteOnClose
    auto dialog = new EngExportDialog(engExporter, m_app);
    dialog->activateWindow();
    dialog->show();
    dialog->raise();
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
void MainWindow::OnSettingsChanged()
{
    m_app.SaveSettings();
    m_StatusBar->showMessage(tr("Settings updated."), 5000);
    m_SimTab->UpdateResults();
    m_PropellantTab->UpdateSettings();
}
void MainWindow::OnTabChanged(int index)
{
    DesignTab* designTab = qobject_cast<DesignTab*>(m_TabWidget->widget(index));
    SimulationTab* simTab = qobject_cast<SimulationTab*>(m_TabWidget->widget(index));

    //a bit of a wonky hack, force the tabs to resize
    if (designTab)
    {
        designTab->resizeEvent(new QResizeEvent(designTab->size(), designTab->size()));
    }
    if (simTab)
    {
        simTab->resizeEvent(new QResizeEvent(simTab->size(), simTab->size()));
    }
}

void MainWindow::OnSelectionChanged(bool isSelected)
{
    m_ActionDelete->setEnabled(isSelected);
}


#pragma clang diagnostic pop