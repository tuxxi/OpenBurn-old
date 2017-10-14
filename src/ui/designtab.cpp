#include "src/ui/designtab.h"

DesignTab::DesignTab(QWidget* parent)
    : QWidget(parent)
{   
    SetupUI();
    connect(newGrainButton, SIGNAL(clicked()), this, SLOT(NewGrainButton_Clicked()));    
}
DesignTab::~DesignTab() 
{

}
void DesignTab::SetupUI()
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);

    newGrainButton = new QPushButton("New Grain", this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(newGrainButton);
    setLayout(layout);
}
void DesignTab::NewGrainButton_Clicked()
{
    grainDialog = new GrainDialog(nullptr, false);
    grainDialog->show();
    grainDialog->activateWindow();
    grainDialog->raise(); 
}