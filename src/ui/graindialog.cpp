#include "graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain)
{
    SetupGraphicsView();    
    SetupUI();
    setAttribute(Qt::WA_DeleteOnClose);

    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));
}
GrainDialog::~GrainDialog()
{
}
void GrainDialog::SetupUI()
{
    if (objectName().isEmpty())
        setObjectName(QStringLiteral("GrainDialog"));

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    QGridLayout* controlsLayout = new QGridLayout;
    QFrame* frame = new QFrame(this);
    frame->setLayout(controlsLayout); 
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);    

    QStringList units = (QStringList() <<
    tr("Inches (in)") <<
    tr("Millimeters (mm)") <<
    tr("Centimeters (cm)") <<
    tr("Feet (ft)") <<
    tr("Meters (m)"));
    
    //Propellant Selection Box 
    m_propellantComboBox = new QComboBox(frame);
    QLabel* label = new QLabel(frame);
    label->setText(tr("Propellant Type"));    
    m_modifyPropellantDatabase = new QToolButton(frame);
    m_modifyPropellantDatabase->setText("...");    
    controlsLayout->addWidget(label, 0, 0);
    controlsLayout->addWidget(m_propellantComboBox, 0, 1);
    controlsLayout->addWidget(m_modifyPropellantDatabase, 0, 2);
    
    // -------------------------
    //Grain Length 
    m_grainLengthSpinBox = new QDoubleSpinBox(frame);
    m_grainLengthSpinBox->setDecimals(3);
    m_grainLengthSpinBox->setSingleStep(0.25);    
    QLabel* label_2 = new QLabel(frame);
    label_2->setText(tr("Grain Length"));    
    m_grainLenUnitsComboBox = new QComboBox(frame);
    m_grainLenUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    m_grainLenUnitsComboBox->addItems(units);

    controlsLayout->addWidget(label_2, 1, 0);
    controlsLayout->addWidget(m_grainLengthSpinBox, 1, 1);
    controlsLayout->addWidget(m_grainLenUnitsComboBox, 1, 2);

    //Grain Diameter
    m_grainDiameterSpinBox = new QDoubleSpinBox(frame);
    m_grainDiameterSpinBox->setDecimals(3);
    m_grainDiameterSpinBox->setSingleStep(0.25);    
    QLabel* label_3 = new QLabel(frame);
    label_3->setText(tr("Grain Diameter"));    
    m_grainDiaUnitsComboBox = new QComboBox(frame);
    m_grainDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    m_grainDiaUnitsComboBox->addItems(units);
    
    controlsLayout->addWidget(label_3, 2, 0);
    controlsLayout->addWidget(m_grainDiameterSpinBox, 2, 1);
    controlsLayout->addWidget(m_grainDiaUnitsComboBox, 2, 2);

    //Grain Core Diameter
    m_grainCoreDiameterSpinBox = new QDoubleSpinBox(frame);
    m_grainCoreDiameterSpinBox->setDecimals(3);
    m_grainCoreDiameterSpinBox->setSingleStep(0.25);
    QLabel* label_4 = new QLabel(frame);
    label_4->setText(tr("Grain Core Diameter"));    
    m_grainCoreDiaUnitsComboBox = new QComboBox(frame);
    m_grainCoreDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    m_grainCoreDiaUnitsComboBox->addItems(units);
    
    controlsLayout->addWidget(label_4, 3, 0);
    controlsLayout->addWidget(m_grainCoreDiameterSpinBox, 3, 1);
    controlsLayout->addWidget(m_grainCoreDiaUnitsComboBox, 3, 2);

    //Inhibited Faces
    m_grainInhibitedFacesSpinBox = new QSpinBox(frame);
    m_grainInhibitedFacesSpinBox->setMaximum(2);
    QLabel* label_5 = new QLabel(frame);
    label_5->setText(tr("Number Inhibited Faces"));    

    controlsLayout->addWidget(label_5, 4, 0);
    controlsLayout->addWidget(m_grainInhibitedFacesSpinBox, 4, 1);

    // OK and Cancel buttons
    m_applyButton = new QPushButton(frame);
    m_cancelButton = new QPushButton(frame);
    QString GrainConfirm = m_isNewGrainWindow ? tr("Add") : tr("Apply");
    m_applyButton->setText(GrainConfirm);
    m_cancelButton->setText(tr("Close"));

    controlsLayout->addWidget(m_applyButton, 5, 0);
    controlsLayout->addWidget(m_cancelButton, 5, 1);
        
    //2D grain cross section preview
    
    QVBoxLayout* masterVLayout = new QVBoxLayout;    
    masterVLayout->addWidget(frame);    
    masterVLayout->addWidget(m_graphicsView);

    setLayout(masterVLayout);
}
void GrainDialog::SetupGraphicsView()
{    
    m_graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_graphicsView->sizePolicy().hasHeightForWidth());
    m_graphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::on_cancelButton_clicked()
{
    close();
    emit DialogClosed();
}

void GrainDialog::on_applyButton_clicked()
{
    if (m_isNewGrainWindow)
    {
        //temp - DONT STACK ALLOCATE THIS !!
        OpenBurnPropellant prop;
        BatesGrain *grain = new BatesGrain(
            m_grainDiameterSpinBox->value(),            
            m_grainCoreDiameterSpinBox->value(),
            m_grainLengthSpinBox->value(),
            prop,                    
            m_grainInhibitedFacesSpinBox->value());
        emit NewGrain(grain);
    }
    else //Editing grain
    {

    }
}
