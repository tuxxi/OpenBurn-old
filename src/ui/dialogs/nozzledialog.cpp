#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QCloseEvent>

#include "src/ui/dialogs/nozzledialog.hpp"
NozzleDialog::NozzleDialog(const OpenBurnNozzle* seed, OpenBurnSettings* settings, QWidget* parent)
    : QDialog(parent),
      m_GlobalSettings(settings)
{
	m_Nozzle = std::move(NozzlePtr(const_cast<OpenBurnNozzle*>(seed)));
	m_OldNozzle = m_Nozzle ? m_Nozzle->Clone() : nullptr; //if we don't have a nozzle to modify, there's nothing to save the state of!
    SetupUI();
    connect(m_btnOK, &QPushButton::clicked,
            this, &NozzleDialog::OnOkButtonClicked);
    connect(m_btnClose, &QPushButton::clicked,
            this, &NozzleDialog::OnCloseButtonClicked);
    connect(m_btnApply, &QPushButton::clicked,
            this, &NozzleDialog::OnApplyButtonClicked);
    
    connect(m_NozzleDesign, &OpenBurnDesignNozzle::DesignUpdated,
            this, &NozzleDialog::OnDesignUpdated);
}

NozzleDialog::~NozzleDialog()
{
	//release the dialog from managing these ptrs since they will be managed by the motor from now on.
	m_Nozzle.release();
	m_OldNozzle.release();
}

void NozzleDialog::closeEvent(QCloseEvent* event)
{
	emit DialogClosed();
	event->accept();
}

void NozzleDialog::SetupUI()
{
    QGridLayout* layout = new QGridLayout(this);
    setWindowTitle(tr("Modify Nozzle"));

    m_gbFrame = new QGroupBox(tr("Nozzle Design"), this);
    m_gbFrame->setLayout(layout);

    auto nozz = dynamic_cast<ConicalNozzle*>(m_Nozzle.get());
    m_NozzleDesign = new ConicalNozzleDesign(this, nozz, m_GlobalSettings);
    m_btnOK = new QPushButton(tr("OK"), this);
    m_btnClose = new QPushButton(tr("Cancel"), this);
    m_btnApply = new QPushButton(tr("Apply"), this);

    //buttons
    layout->addWidget(m_NozzleDesign, 0, 0, 1, 3);
    layout->addWidget(m_btnOK, 255, 0);
    layout->addWidget(m_btnApply, 255, 1);        
    layout->addWidget(m_btnClose, 255, 2);
    
    QVBoxLayout* masterLayout = new QVBoxLayout(this);
    
    masterLayout->addWidget(m_gbFrame);
    setLayout(masterLayout);
}
void NozzleDialog::OnDesignUpdated()
{
    ConicalNozzleDesign* design = dynamic_cast<ConicalNozzleDesign*>(m_NozzleDesign);
    if (design)
    {
        if (!m_Nozzle)
        {
            m_Nozzle = std::make_unique<ConicalNozzle>(
                design->GetThroatDiameter(), 
                design->GetExitDiameter(), 
                design->GetDivergentHalfAngle());
        }
        else
        {
            auto nozz = static_cast<ConicalNozzle*>(m_Nozzle.get());
            nozz->SetNozzleThroat(design->GetThroatDiameter());
            nozz->SetNozzleExit(design->GetExitDiameter());
            nozz->SetHalfAngle(design->GetDivergentHalfAngle());
        }
    }
}
void NozzleDialog::OnApplyButtonClicked()
{
    Accept();
}
void NozzleDialog::OnCloseButtonClicked()
{
    close();
}
void NozzleDialog::OnOkButtonClicked()
{
    Accept();
    close();
}
void NozzleDialog::Accept()
{
    //OPENBURN_TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_NozzleDesign->GetThroatDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Throat Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_NozzleDesign->GetExitDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Nozzle Exit Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    OnDesignUpdated();
	//if the old nozzle is null, we never had a seed to begin with, so it must be a newly created nozzle
	if (m_OldNozzle == nullptr)
	{
		emit NewNozzle(m_Nozzle);
	}
	else
	{
		emit NozzleModified(m_Nozzle, m_OldNozzle);
	}
}
