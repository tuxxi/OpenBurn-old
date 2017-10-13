#include "graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain)
{
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

    frame = new QFrame(this);
    masterVLayout = new QVBoxLayout;

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
    
    controlsVLayout = new QVBoxLayout();

    //Propellant Selection Box 
    propellantComboBox = new QComboBox(frame);
    label = new QLabel(frame);
    modifyPropellantDatabase = new QToolButton(frame);

    propellantHLayout = new QHBoxLayout();
    propellantHLayout->addWidget(propellantComboBox);
    propellantHLayout->addWidget(label);
    propellantHLayout->addWidget(modifyPropellantDatabase);
    propellantHLayout->setStretch(0, 5);
    propellantHLayout->setStretch(1, 1);
    
    // -------------------------
    controlsVLayout->addLayout(propellantHLayout);

    //Grain Length 
    grainLengthHLayout = new QHBoxLayout();

    grainLengthComboBox = new QDoubleSpinBox(frame);
    grainLengthComboBox->setDecimals(3);
    grainLengthComboBox->setMaximum(1e+9);
    grainLengthComboBox->setSingleStep(0.25);    
    label_2 = new QLabel(frame);
    grainLenUnitsComboBox = new QComboBox(frame);
    grainLenUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainLenUnitsComboBox->addItems(units);
    
    grainLengthHLayout->addWidget(grainLengthComboBox);    
    grainLengthHLayout->addWidget(grainLenUnitsComboBox);
    grainLengthHLayout->addWidget(label_2);

    // -------------------------
    controlsVLayout->addLayout(grainLengthHLayout);

    //Grain Diameter
    grainDiaHLayout = new QHBoxLayout();

    grainDiameterComboBox = new QDoubleSpinBox(frame);
    grainDiameterComboBox->setDecimals(3);
    grainDiameterComboBox->setMaximum(1e+9);
    grainDiameterComboBox->setSingleStep(0.25);    
    label_3 = new QLabel(frame);
    grainDiaUnitsComboBox = new QComboBox(frame);
    grainDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainDiaUnitsComboBox->addItems(units);
    
    
    grainDiaHLayout->addWidget(grainDiameterComboBox);
    grainDiaHLayout->addWidget(label_3);
    grainDiaHLayout->addWidget(grainDiaUnitsComboBox);

    // ------------------------
    controlsVLayout->addLayout(grainDiaHLayout);

    //Grain Core Diameter
    grainCoreDiaHLayout = new QHBoxLayout();

    grainCoreDiameterComboBox = new QDoubleSpinBox(frame);
    grainCoreDiameterComboBox->setDecimals(3);
    grainCoreDiameterComboBox->setMaximum(1e+9);
    grainCoreDiameterComboBox->setSingleStep(0.25);
    label_4 = new QLabel(frame);
    grainCoreDiaUnitsComboBox = new QComboBox(frame);
    grainCoreDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    grainCoreDiaUnitsComboBox->addItems(units);
    
    grainCoreDiaHLayout->addWidget(label_4);    
    grainCoreDiaHLayout->addWidget(grainCoreDiameterComboBox);    
    grainCoreDiaHLayout->addWidget(grainCoreDiaUnitsComboBox);

    // -----------
    controlsVLayout->addLayout(grainCoreDiaHLayout);

    //Inhibited Faces
    grainFacesHLayout = new QHBoxLayout();

    grainInhibitedFacesComboBox = new QSpinBox(frame);
    grainInhibitedFacesComboBox->setMaximum(2);
    label_5 = new QLabel(frame);
    horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    grainFacesHLayout->addWidget(label_5);
    grainFacesHLayout->addWidget(grainInhibitedFacesComboBox);
    grainFacesHLayout->addItem(horizontalSpacer_2);

    // ---------------
    controlsVLayout->addLayout(grainFacesHLayout);

    // OK and Cancel buttons
    commandButtonsHLayout = new QHBoxLayout();
    commandButtonsHLayout->setSpacing(6);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    ApplyOrAddNewGrainButton = new QPushButton(frame);
    cancelButton = new QPushButton(frame);

    commandButtonsHLayout->addItem(horizontalSpacer);
    commandButtonsHLayout->addWidget(ApplyOrAddNewGrainButton);
    commandButtonsHLayout->addWidget(cancelButton);
    
    controlsVLayout->addLayout(commandButtonsHLayout);
    
    //2D grain cross section preview
    graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
    graphicsView->setSizePolicy(sizePolicy2);

    masterVLayout->addWidget(frame, 0, Qt::AlignTop);    
    masterVLayout->addWidget(graphicsView);

    setLayout(masterVLayout);

    setWindowTitle(tr("Dialog"));
    label->setText(tr("Propellant Type"));
    modifyPropellantDatabase->setText(tr("..."));
    label_2->setText(tr("Grain Length"));
    label_3->setText(tr("Grain Diameter"));
    label_4->setText(tr("Grain Core Diameter"));
    label_5->setText(tr("Number Inhibited Faces"));

    QString GrainConfirm = m_isNewGrainWindow ? tr("Add") : tr("Apply");
    ApplyOrAddNewGrainButton->setText(GrainConfirm);
    cancelButton->setText(tr("Close"));

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
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
                    grainDiameterComboBox->value(),
                    grainCoreDiameterComboBox->value(),
                    grainLengthComboBox->value(),
                    prop);
        emit NewGrain(grain);
    }
    else //Editing grain
    {

    }
}
