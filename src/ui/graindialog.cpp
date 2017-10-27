#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

#include "graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(QWidget *parent, OpenBurnGrain* grain) :
    QDialog(parent), m_isNewGrainWindow(grain == nullptr), m_Grain(grain)
{
    SetupUI();
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));
    connect(m_grainTypeComboBox, SIGNAL(currentIndexChanged(const QString& )), 
        this, SLOT(on_grainType_changed(const QString&)));
    if (!m_isNewGrainWindow) //only "seed" the values to edit if we're NOT making a _new_ grain.
    {
        qDebug() << "seeding values...";
        SeedValues();
    }
}
GrainDialog::~GrainDialog()
{
}
void GrainDialog::SetupUI()
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("GrainDialog"));        
    }

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    controlsLayout = new QGridLayout;
    frame = new QFrame(this);
    frame->setLayout(controlsLayout); 
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);    
    
    //Grain type combo box
    m_grainTypeComboBox = new QComboBox(frame);
    m_grainTypeComboBox->addItems(QStringList() 
        << tr("Cylindrical core (BATES)")
        << tr("test"));
    QLabel* label_0 = new QLabel(frame);
    label_0->setBuddy(m_grainTypeComboBox);
    label_0->setText(tr("Grain Core Type"));
    controlsLayout->addWidget(label_0, 0, 0);
    controlsLayout->addWidget(m_grainTypeComboBox, 0, 1);

    //Propellant Selection Box 
    m_propellantComboBox = new QComboBox(frame);
    QLabel* label = new QLabel(frame);
    label->setBuddy(m_propellantComboBox);        
    label->setText(tr("Propellant Type"));
    m_modifyPropellantDatabase = new QToolButton(frame);
    m_modifyPropellantDatabase->setText("...");    
    controlsLayout->addWidget(label, 1, 0);
    controlsLayout->addWidget(m_propellantComboBox, 1, 1);
    controlsLayout->addWidget(m_modifyPropellantDatabase, 1, 2);
    
    //Grain Length 
    m_grainLengthSpinBox = new QDoubleSpinBox(frame);
    m_grainLengthSpinBox->setDecimals(3);
    m_grainLengthSpinBox->setSingleStep(0.25);    
    QLabel* label_2 = new QLabel(frame);
    label_2->setBuddy(m_grainLengthSpinBox);
    label_2->setText(tr("Grain Length"));    
    m_grainLenUnitsComboBox = new QComboBox(frame);
    m_grainLenUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    m_grainLenUnitsComboBox->addItems(OpenBurnUtil::g_kLengthUnits);

    controlsLayout->addWidget(label_2, 2, 0);
    controlsLayout->addWidget(m_grainLengthSpinBox, 2, 1);
    controlsLayout->addWidget(m_grainLenUnitsComboBox, 2, 2);

    //Grain Diameter
    m_grainDiameterSpinBox = new QDoubleSpinBox(frame);
    m_grainDiameterSpinBox->setDecimals(3);
    m_grainDiameterSpinBox->setSingleStep(0.25);    
    QLabel* label_3 = new QLabel(frame);
    label_3->setBuddy(m_grainDiameterSpinBox);
    label_3->setText(tr("Grain Diameter"));    
    m_grainDiaUnitsComboBox = new QComboBox(frame);
    m_grainDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
    m_grainDiaUnitsComboBox->addItems(OpenBurnUtil::g_kLengthUnits);
    
    controlsLayout->addWidget(label_3, 3, 0);
    controlsLayout->addWidget(m_grainDiameterSpinBox, 3, 1);
    controlsLayout->addWidget(m_grainDiaUnitsComboBox, 3, 2);
    //Inhibited Faces
    m_grainInhibitedFacesSpinBox = new QSpinBox(frame);
    m_grainInhibitedFacesSpinBox->setMaximum(2);
    QLabel* label_5 = new QLabel(frame);
    label_5->setBuddy(m_grainInhibitedFacesSpinBox);
    label_5->setText(tr("Number Inhibited Faces"));    

    controlsLayout->addWidget(label_5, 10, 0);
    controlsLayout->addWidget(m_grainInhibitedFacesSpinBox, 10, 1);    

    // OK and Cancel buttons
    m_applyButton = new QPushButton(frame);
    m_cancelButton = new QPushButton(frame);
    m_applyButton->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_cancelButton->setText(tr("Close"));

    controlsLayout->addWidget(m_applyButton, 11, 0);
    controlsLayout->addWidget(m_cancelButton, 11, 1);
    
    setTabOrder(m_grainTypeComboBox, m_propellantComboBox);
    setTabOrder(m_propellantComboBox, m_grainLengthSpinBox);
    setTabOrder(m_grainLengthSpinBox, m_grainDiameterSpinBox);
    setTabOrder(m_grainDiameterSpinBox, m_grainInhibitedFacesSpinBox);
    setTabOrder(m_grainInhibitedFacesSpinBox, m_applyButton);
    setTabOrder(m_applyButton, m_cancelButton);
    setTabOrder(m_cancelButton, m_grainTypeComboBox); //loop back to top

    QVBoxLayout* masterVLayout = new QVBoxLayout;    
    masterVLayout->addWidget(frame);    
    SetupGraphicsView();
    masterVLayout->addWidget(m_graphicsView);

    setLayout(masterVLayout);
    RefreshUI();
}
void GrainDialog::RefreshUI()
{
    qDebug() << "refreshing UI";
    m_GrainType = static_cast<GRAINTYPE>(m_grainTypeComboBox->currentIndex());    
    if (m_GrainType == GRAINTYPE_BATES)
    {
        qDebug() << " bates grain!";
        //Grain Core Diameter
        m_grainCoreDiameterSpinBox = new QDoubleSpinBox(frame);
        m_grainCoreDiameterSpinBox->setDecimals(3);
        m_grainCoreDiameterSpinBox->setSingleStep(0.25);
        QLabel* label_4 = new QLabel(frame);
        label_4->setText(tr("Grain Core Diameter"));    
        m_grainCoreDiaUnitsComboBox = new QComboBox(frame);
        m_grainCoreDiaUnitsComboBox->setLayoutDirection(Qt::LeftToRight);
        m_grainCoreDiaUnitsComboBox->addItems(OpenBurnUtil::g_kLengthUnits);
        
        controlsLayout->addWidget(label_4, 4, 0);
        controlsLayout->addWidget(m_grainCoreDiameterSpinBox, 4, 1);
        controlsLayout->addWidget(m_grainCoreDiaUnitsComboBox, 4, 2);
        setTabOrder(m_grainDiameterSpinBox, m_grainCoreDiameterSpinBox);        
        setTabOrder(m_grainCoreDiameterSpinBox, m_grainInhibitedFacesSpinBox);        
    }
}
void GrainDialog::SetupGraphicsView()
{    
    m_graphicsView = new QGraphicsView(this);
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_graphicsView->sizePolicy().hasHeightForWidth());
    m_graphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::SeedValues()
{
    m_grainLengthSpinBox->setValue(m_Grain->GetLength());
    m_grainDiameterSpinBox->setValue(m_Grain->GetDiameter());
    m_grainInhibitedFacesSpinBox->setValue(m_Grain->GetInhibitedFaces());

    //propellant .. 

    //now we seed the grain type and any special info for every grain type
    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(m_Grain))
    {
        m_grainCoreDiameterSpinBox->setValue(bates->GetCoreDiameter());
        m_GrainType = GRAINTYPE_BATES;
    }
    m_grainTypeComboBox->setCurrentIndex(static_cast<int>(m_GrainType));    
}
void GrainDialog::on_grainType_changed(const QString& text)
{
    Q_UNUSED(text);
    RefreshUI();
}
void GrainDialog::on_cancelButton_clicked()
{
    close();
}
void GrainDialog::on_applyButton_clicked()
{
    m_GrainType = static_cast<GRAINTYPE>(m_grainTypeComboBox->currentIndex());    
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_grainLengthSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Length cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_grainDiameterSpinBox->value())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (m_isNewGrainWindow)
    {
        OpenBurnPropellant* prop = new OpenBurnPropellant;
        BatesGrain *grain = new BatesGrain(
            m_grainDiameterSpinBox->value(),            
            m_grainCoreDiameterSpinBox->value(),
            m_grainLengthSpinBox->value(),
            prop,                    
            m_grainInhibitedFacesSpinBox->value());
        emit SIG_DIALOG_NewGrain(grain);
    }
    else //Editing grain
    {
        //edit grain
        m_Grain->SetDiameter(m_grainDiameterSpinBox->value());
        m_Grain->SetLength(m_grainLengthSpinBox->value());
        m_Grain->SetInhibitedFaces(m_grainInhibitedFacesSpinBox->value());
        //m_Grain->SetPropellantType();
        if (m_GrainType == GRAINTYPE_BATES)
        {
            //use the grain type enum and stuff here because dynamic_cast is expensive 
            static_cast<BatesGrain*>(m_Grain)->SetCoreDiameter(m_grainCoreDiameterSpinBox->value());
        }
        emit SIG_DIALOG_EditGrain(m_Grain);
    }
}
