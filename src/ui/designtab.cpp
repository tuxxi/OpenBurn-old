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
    m_grainsDisplay->setColumnCount(5); //propellant, len, core dia, dia, inhibited face

    QStringList tableHeader = (QStringList() << 
        tr("Length") << 
        tr("Diameter") << 
        tr("Core Diameter") << 
        tr("Propellant") << 
        tr("Inhibited Faces"));
    m_grainsDisplay->setHorizontalHeaderLabels(tableHeader);
    
    m_newGrainButton = new QPushButton("New Grain", this);
    QVBoxLayout *vLayout = new QVBoxLayout;    
    vLayout->addWidget(m_newGrainButton);
    frame->setLayout(vLayout);

    //master layout
    QHBoxLayout* layout = new QHBoxLayout;    
    layout->addWidget(m_grainsDisplay);
    layout->addWidget(frame);
    setLayout(layout);
}
void DesignTab::DialogClosed()
{
    delete m_grainDialog;
}
void DesignTab::NewGrain(OpenBurnGrain *grain)
{

}
void DesignTab::NewGrainButton_Clicked()
{
    m_grainDialog = new GrainDialog(nullptr, false);
    connect(m_grainDialog, SIGNAL(DialogClosed()), this, SLOT(DialogClosed()));
    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise(); 
}