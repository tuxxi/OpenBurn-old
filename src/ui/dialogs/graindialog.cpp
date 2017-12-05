#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

#include "src/ui/dialogs/graindialog.h"
#include "src/grain.h"

GrainDialog::GrainDialog(PropellantList* prop, OpenBurnGrain* seedValues, OpenBurnSettings *settings,
	const GrainVector& grains, QWidget *parent)
    : QDialog(parent),
    m_gfxGrain(nullptr),
    m_grainsToEdit(grains),
    m_Propellants(prop),
    m_GlobalSettings(settings),
    m_isNewGrainWindow(grains.empty())
{
    SetupGraphics();
    SetupUI(seedValues); //setup the ui and populate the various options with the "seed" values
    connect(m_btnCancel, &QPushButton::clicked,
            this, &GrainDialog::OnCancelButtonClicked);
    connect(m_btnApply, &QPushButton::clicked,
            this, &GrainDialog::OnApplyButtonClicked);
    connect(m_GrainDesign, &OpenBurnDesignGrain::GrainDesignChanged,
            this, &GrainDialog::OnDesignUpdated);
}
void GrainDialog::SetupUI(OpenBurnGrain* seed)
{
	setObjectName(QStringLiteral("GrainDialog"));

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));        
    resize(400, 400);
    
    m_layControls = new QGridLayout;
    m_gbFrame = new QGroupBox(tr("Grain Design"));
    m_gbFrame->setLayout(m_layControls);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(1);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);    
    CylindricalGrain* BatesSeed = dynamic_cast<CylindricalGrain*>(seed);

    //Default to BATES grain design mode
    m_GrainDesign = new CylindricalGrainDesign(m_Propellants, BatesSeed, m_GlobalSettings, m_gbFrame);

    // OK and Cancel buttons
    m_btnApply = new QPushButton(m_gbFrame);
    m_btnCancel = new QPushButton(m_gbFrame);
    m_btnApply->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_btnCancel->setText(tr("Close"));

    m_layControls->addWidget(m_GrainDesign, 1, 0, 3, 3);
    //we use a value of 256 here to ensure that we can have a LOT of extra info padded in there.
    m_layControls->addWidget(m_btnApply, 256, 0);
    m_layControls->addWidget(m_btnCancel, 256, 1);

    QVBoxLayout* masterVLayout = new QVBoxLayout;
    masterVLayout->addWidget(m_gbFrame);
    masterVLayout->addWidget(m_GraphicsView);
    setLayout(masterVLayout);
    OnDesignUpdated();
}
void GrainDialog::SetupGraphics()
{
    m_GraphicsView = new QGraphicsView;
    m_GraphicsScene = new QGraphicsScene;
    m_GraphicsView->setScene(m_GraphicsScene);
    m_GraphicsView->show();
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(true);
    m_GraphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::OnDesignUpdated()
{
    CylindricalGrainDesign* design = dynamic_cast<CylindricalGrainDesign*>(m_GrainDesign);
    if (design)
    {
        if (m_isNewGrainWindow && m_grainsToEdit.empty())
        {
			auto grain = std::make_shared<CylindricalGrain>(
                design->GetDiameter(),
                design->GetCoreDiameter(),
                design->GetLength(),
                design->GetPropellant(),                    
				design->GetInhibitedFaces());
				m_grainsToEdit.emplace_back(std::move(grain));
        }
        else
        {
            for (auto& i : m_grainsToEdit)
            {
                CylindricalGrain* grain = static_cast<CylindricalGrain*>(i.get());
                grain->SetDiameter(design->GetDiameter());
                grain->SetCoreDiameter(design->GetCoreDiameter());
                grain->SetLength(design->GetLength());
                grain->SetInhibitedFaces(design->GetInhibitedFaces());
                grain->SetPropellantType(design->GetPropellant());
            }
        }
    }
    UpdateGraphics();
}
void GrainDialog::UpdateGraphics()
{
    if (!m_gfxGrain)
    {
        m_gfxGrain = std::make_unique<GrainGraphicsItem>(m_grainsToEdit.begin()->get(), 100, false);
        m_GraphicsScene->addItem(m_gfxGrain.get());
    }
    m_gfxGrain->setPos(0, 0);    
    m_gfxGrain->update(m_gfxGrain->boundingRect());
    m_GraphicsView->viewport()->repaint();

    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_GraphicsView->setSceneRect(m_gfxGrain->boundingRect());
    QRectF bounds = QRectF(m_gfxGrain->boundingRect().left(), m_gfxGrain->boundingRect().top(), 
        m_gfxGrain->boundingRect().width() + 50, m_gfxGrain->boundingRect().height() + 50);
    m_GraphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_GraphicsView->viewport()->repaint();
    m_gfxGrain->update(m_gfxGrain->boundingRect());    
}
void GrainDialog::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    UpdateGraphics();
}
void GrainDialog::closeEvent(QCloseEvent* event)
{
	emit DialogClosed();
	event->accept();
}

void GrainDialog::OnCancelButtonClicked()
{
    close();
}
void GrainDialog::OnApplyButtonClicked()
{
    //TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_GrainDesign->GetLength()) )
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Length cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_GrainDesign->GetDiameter())) 
    {
        QMessageBox::warning(this, tr("OpenBurn: Warning!"),
        tr("Grain Diameter cannot be 0!\n"),
        QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    OnDesignUpdated();
    if (m_isNewGrainWindow)
    {
        emit GrainAdded(std::move(*m_grainsToEdit.begin()));
		m_grainsToEdit.pop_back();
    }
    else
    {
        for (auto& i : m_grainsToEdit)
        {
            emit GrainEdited(i);
        }
    }
}
