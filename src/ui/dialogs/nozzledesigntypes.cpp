#include <QLabel>

#include "nozzledesigntypes.h"
#include "src/util.h"

OpenBurnDesignNozzle::OpenBurnDesignNozzle(QWidget* parent, OpenBurnNozzle* nozz)
    : QWidget(parent), m_seedNozzle(nozz)
{
    SetupUI();
}
void OpenBurnDesignNozzle::SetupUI()
{
    layout = new QGridLayout(this);
    //nozzle type
    m_nozzleType = new QComboBox(this);
    m_nozzleType->addItem(tr("Conical Nozzle"));
    QLabel* label_0 = new QLabel(tr("Nozzle Type"));
    layout->addWidget(label_0, 0, 0);
    layout->addWidget(m_nozzleType, 0, 1);

    //throat diameter
    m_throatDiaSpinBox = new QDoubleSpinBox(this);
    m_throatDiaSpinBox->setDecimals(3);
    m_throatDiaSpinBox->setSingleStep(0.25f);
    QLabel* label = new QLabel(tr("Throat Diameter"), this);
    m_throatDiaUnits = new QComboBox(this);
    m_throatDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label, 1, 0);
    layout->addWidget(m_throatDiaSpinBox, 1, 1);
    layout->addWidget(m_throatDiaUnits, 1, 2);

    //exit (expansion, divergence) diameter
    m_exitDiaSpinBox = new QDoubleSpinBox(this);
    m_exitDiaSpinBox->setDecimals(3);
    m_exitDiaSpinBox->setSingleStep(0.25f);
    QLabel* label_2 = new QLabel(tr("Exit Diameter"), this);
    m_exitDiaUnits = new QComboBox(this);
    m_exitDiaUnits->addItems(OpenBurnUtil::g_kLengthUnits);
    layout->addWidget(label_2, 2, 0);
    layout->addWidget(m_exitDiaSpinBox, 2, 1);
    layout->addWidget(m_exitDiaUnits, 2, 2);
    
    setTabOrder(m_throatDiaSpinBox, m_exitDiaSpinBox);

}
OpenBurnDesignNozzle::~OpenBurnDesignNozzle()
{

}
void OpenBurnDesignNozzle::SeedValues()
{
    if (m_seedNozzle)
    {
        m_throatDiaSpinBox->setValue(m_seedNozzle->GetNozzleThroat());
        m_exitDiaSpinBox->setValue(m_seedNozzle->GetNozzleExit());
    }
}
double OpenBurnDesignNozzle::GetThroatDiameter()
{
    return m_throatDiaSpinBox->value();
}
double OpenBurnDesignNozzle::GetExitDiameter()
{
    return m_exitDiaSpinBox->value();
}
OpenBurnNozzle* OpenBurnDesignNozzle::GetNozzle()
{
    return m_seedNozzle;
}
void OpenBurnDesignNozzle::AddNewControls(QWidget* widget, int row, int col)
{
    layout->addWidget(widget, row+3, col);
}

ConicalNozzleDesign::ConicalNozzleDesign(QWidget* parent, ConicalNozzle* nozz)
    : OpenBurnDesignNozzle(parent, nozz)
{
    m_halfAngleSpinBox = new QDoubleSpinBox(this);
    m_halfAngleSpinBox->setDecimals(3);
    m_halfAngleSpinBox->setSingleStep(0.25);
    QLabel* label_3 = new QLabel(tr("Divergent Half Angle"), this);
    m_halfAngleUnits = new QComboBox(this);
    m_halfAngleUnits->setLayoutDirection(Qt::LeftToRight);
    m_halfAngleUnits->addItems(OpenBurnUtil::g_kAngleUnits);

    AddNewControls(label_3, 0, 0);
    AddNewControls(m_halfAngleSpinBox, 0, 1);
    AddNewControls(m_halfAngleUnits, 0, 2);
    SeedValues();
}
ConicalNozzleDesign::~ConicalNozzleDesign()
{

}
void ConicalNozzleDesign::SeedValues()
{
    if ( ConicalNozzle* nozz = (dynamic_cast<ConicalNozzle*>(m_seedNozzle)))
    {
        m_halfAngleSpinBox->setValue(nozz->GetHalfAngle());
    }
}
double ConicalNozzleDesign::GetDivergentHalfAngle()
{
    return m_halfAngleSpinBox->value();
}