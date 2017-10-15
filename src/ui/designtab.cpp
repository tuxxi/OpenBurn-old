#include "src/ui/designtab.h"

DesignTab::DesignTab(QWidget* parent)
    : QWidget(parent)
{   
    SetupUI();
    connect(m_newGrainButton, SIGNAL(clicked()), this, SLOT(NewGrainButton_Clicked()));
}
DesignTab::~DesignTab() 
{

}
void DesignTab::SetupUI()
{
    //controls frame
    QGroupBox* frame = new QGroupBox(tr("Grain Design"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);    

    m_grainsDisplay = new GrainTableWidget(this);
    m_grainsDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_grainsDisplay->setColumnCount(5); //propellant, len, core dia, dia, inhibited face

    QStringList tableHeader = (QStringList() << 
        tr("Length") << 
        tr("Diameter") << 
        tr("Core Diameter") << 
        tr("Propellant") << 
        tr("Inhibited Faces"));
    m_grainsDisplay->setHorizontalHeaderLabels(tableHeader);
    
    m_newGrainButton = new QPushButton(tr("New Grain"));
    QVBoxLayout *vLayout = new QVBoxLayout;    
    vLayout->addWidget(m_newGrainButton);
    frame->setLayout(vLayout);

    //master layout
    QGridLayout* layout = new QGridLayout;    
    layout->addWidget(m_grainsDisplay, 0, 0);
    layout->addWidget(frame, 0, 1);
    
    m_grainCrossSection = new QGraphicsView;
    layout->addWidget(m_grainCrossSection, 1, 0, 1, 2);
    setLayout(layout);
}
void DesignTab::SLOT_DialogClosed()
{
    delete m_grainDialog;
}
void DesignTab::SLOT_NewGrain(OpenBurnGrain *grain)
{
    int numItems = m_grainsDisplay->rowCount();
    m_grainsDisplay->setRowCount(numItems+1);
    m_grainsDisplay->setItem(numItems, 0, new QTableWidgetItem(QString::number(grain->GetLength())));
    m_grainsDisplay->setItem(numItems, 1, new QTableWidgetItem(QString::number(grain->GetDiameter())));
    m_grainsDisplay->setItem(numItems, 2, new QTableWidgetItem(QString::number(grain->GetCoreDiameter())));    
    m_grainsDisplay->setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType().GetPropellantName()));
    m_grainsDisplay->setItem(numItems, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));
    
    emit SIG_NewGrain(grain);    
}
void DesignTab::NewGrainButton_Clicked()
{
    m_grainDialog = new GrainDialog(nullptr, true);
    connect(m_grainDialog, SIGNAL(SIG_DialogClosed()), this, SLOT(SLOT_DialogClosed()));
    connect(m_grainDialog, SIGNAL(SIG_DIALOG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_NewGrain(OpenBurnGrain*)));        
    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise(); 
}