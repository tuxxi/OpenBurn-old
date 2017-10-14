#include "graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain)
{
    SetupGraphicsView();    
    SetupUI();
    setAttribute(Qt::WA_DeleteOnClose);
}
void GrainDialog::SetupUI()
{
    if (objectName().isEmpty())
        setObjectName(QStringLiteral("GrainDialog"));

    resize(400, 400);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);

    QFrame* frame = new QFrame(this);
    QVBoxLayout* masterVLayout = new QVBoxLayout;
    
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy1);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QStringList units = (QStringList() <<
    tr("inches", "Inches (in)") <<
    tr("millimeters", "Millimeters (mm)") <<
    tr("centimeters", "Centimeters (cm)") <<
    tr("feet", "Feet (ft)") <<
    tr("meters", "Meters (m)"));
    
    QVBoxLayout* controlsVLayout = new QVBoxLayout();
    
    //Propellant Selection Box 
    propellantComboBox = new QComboBox(frame);
    QLabel* label = new QLabel(frame);
    modifyPropellantDatabase = new QToolButton(frame);
    modifyPropellantDatabase->setText(tr("..."));    

    QHBoxLayout* propellantHLayout = new QHBoxLayout();

    propellantHLayout->addWidget(label);    
    propellantHLayout->addWidget(propellantComboBox);
    propellantHLayout->addWidget(modifyPropellantDatabase);
    propellantHLayout->setStretch(0, 5);
    propellantHLayout->setStretch(1, 1);
    
    // -------------------------
    controlsVLayout->addLayout(propellantHLayout);

    //Grain Length 
    QHBoxLayout* grainLengthHLayout = new QHBoxLayout();

    grainLengthSpinBox = new QDoubleSpinBox(frame);
    grainLengthSpinBox->setDecimals(3);
    grainLengthSpinBox->setMaximum(1e+9);
    grainLengthSpinBox->setSingleStep(0.25);    
    QLabel* label_2 = new QLabel(frame);
    grainLenUnitsComboBox = new QComboBox(frame);
    grainLenUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainLenUnitsComboBox->addItems(units);

    grainLengthHLayout->addWidget(label_2);    
    grainLengthHLayout->addWidget(grainLengthSpinBox);    
    grainLengthHLayout->addWidget(grainLenUnitsComboBox);

    // -------------------------
    controlsVLayout->addLayout(grainLengthHLayout);

    //Grain Diameter
    QHBoxLayout* grainDiaHLayout = new QHBoxLayout();

    grainDiameterSpinBox = new QDoubleSpinBox(frame);
    grainDiameterSpinBox->setDecimals(3);
    grainDiameterSpinBox->setMaximum(1e+9);
    grainDiameterSpinBox->setSingleStep(0.25);    
    QLabel* label_3 = new QLabel(frame);
    grainDiaUnitsComboBox = new QComboBox(frame);
    grainDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainDiaUnitsComboBox->addItems(units);
    
    grainDiaHLayout->addWidget(label_3);    
    grainDiaHLayout->addWidget(grainDiameterSpinBox);
    grainDiaHLayout->addWidget(grainDiaUnitsComboBox);

    // ------------------------
    controlsVLayout->addLayout(grainDiaHLayout);

    //Grain Core Diameter
    QHBoxLayout* grainCoreDiaHLayout = new QHBoxLayout();

    grainCoreDiameterSpinBox = new QDoubleSpinBox(frame);
    grainCoreDiameterSpinBox->setDecimals(3);
    grainCoreDiameterSpinBox->setMaximum(1e+9);
    grainCoreDiameterSpinBox->setSingleStep(0.25);
    QLabel* label_4 = new QLabel(frame);
    grainCoreDiaUnitsComboBox = new QComboBox(frame);
    grainCoreDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainCoreDiaUnitsComboBox->addItems(units);
    
    grainCoreDiaHLayout->addWidget(label_4);    
    grainCoreDiaHLayout->addWidget(grainCoreDiameterSpinBox);    
    grainCoreDiaHLayout->addWidget(grainCoreDiaUnitsComboBox);

    // -----------
    controlsVLayout->addLayout(grainCoreDiaHLayout);

    //Inhibited Faces
    QHBoxLayout* grainFacesHLayout = new QHBoxLayout();

    grainInhibitedFacesSpinBox = new QSpinBox(frame);
    grainInhibitedFacesSpinBox->setMaximum(2);
    QLabel* label_5 = new QLabel(frame);
    QSpacerItem* horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    grainFacesHLayout->addWidget(label_5);
    grainFacesHLayout->addWidget(grainInhibitedFacesSpinBox);
    grainFacesHLayout->addItem(horizontalSpacer_2);

    // ---------------
    controlsVLayout->addLayout(grainFacesHLayout);

    // OK and Cancel buttons
    QHBoxLayout* commandButtonsHLayout = new QHBoxLayout();
    commandButtonsHLayout->setSpacing(6);
    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    ApplyOrAddNewGrainButton = new QPushButton(frame);
    cancelButton = new QPushButton(frame);

    commandButtonsHLayout->addItem(horizontalSpacer);
    commandButtonsHLayout->addWidget(ApplyOrAddNewGrainButton);
    commandButtonsHLayout->addWidget(cancelButton);
    
    controlsVLayout->addLayout(commandButtonsHLayout);
    
    //2D grain cross section preview

    frame->setLayout(controlsVLayout);    
    masterVLayout->addWidget(frame);    
    masterVLayout->addWidget(graphicsView);

    setLayout(masterVLayout);

    setWindowTitle(tr("Dialog"));
    label->setText(tr("Propellant Type"));
    label_2->setText(tr("Grain Length"));
    label_3->setText(tr("Grain Diameter"));
    label_4->setText(tr("Grain Core Diameter"));
    label_5->setText(tr("Number Inhibited Faces"));

    QString GrainConfirm = m_isNewGrainWindow ? tr("Add") : tr("Apply");
    ApplyOrAddNewGrainButton->setText(GrainConfirm);
    cancelButton->setText(tr("Close"));

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
}
void GrainDialog::SetupGraphicsView()
{    
    graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
    graphicsView->setSizePolicy(sizePolicy2);


}
GrainDialog::~GrainDialog()
{
}
void GrainDialog::on_cancelButton_clicked()
{
    close();
    emit DialogClosed();
}

void GrainDialog::on_ApplyOrAddNewGrainButton_clicked()
{
    if (m_isNewGrainWindow)
    {
        OpenBurnPropellant prop;
        BatesGrain *grain = new BatesGrain(
                    grainDiameterSpinBox->value(),
                    grainCoreDiameterSpinBox->value(),
                    grainLengthSpinBox->value(),
                    prop);
        emit NewGrain(grain);
    }
    else //Editing grain
    {

    }
}
