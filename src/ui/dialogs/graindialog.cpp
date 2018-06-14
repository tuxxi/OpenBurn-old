#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>

#include <QDebug>

#include "src/ui/dialogs/graindialog.hpp"
#include "src/application.hpp"

GrainDialog::GrainDialog(OpenBurnApplication& app, QWidget *parent)
    : QDialog(parent),
    m_gfxGrain(nullptr),
    m_GrainDesign(nullptr),
    m_app(app)
    //m_isNewGrainWindow(grains.empty())
{
    //we clone the seed if it exists, otherwise just use a default cyl grain

    SetupGraphics();
    SetupUI();
    //SeedValues(m_seedGrain);
    //new function pointer syntax does not work for overloaded signals or slots >.<
    connect(m_sbGrainDia, SIGNAL(valueChanged(double)),
            this, SLOT(OnDesignUpdated()));
    connect(m_sbGrainLen, SIGNAL(valueChanged(double)),
            this, SLOT(OnDesignUpdated()));
    connect(m_sbGrainInhibit, SIGNAL(valueChanged(int)),
            this, SLOT(OnDesignUpdated()));

    connect(m_btnCancel, &QPushButton::clicked,
            this, &GrainDialog::OnCancelButtonClicked);
    connect(m_btnApply, &QPushButton::clicked,
            this, &GrainDialog::OnApplyButtonClicked);
    connect(m_cbGrainType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(OnGrainCoreTypeChanged(int)));

}
void GrainDialog::SetupUI()
{
    setObjectName(QString("GrainDialog"));

    setWindowTitle(m_isNewGrainWindow ? tr("Add New Grain") : tr("Modify Grain"));
    resize(400, 400);

    m_layControls = new QGridLayout;
    m_gbFrame = new QGroupBox(tr("Grain Design"));
    m_gbFrame->setLayout(m_layControls);

    //Grain type combo box
    m_cbGrainType = new QComboBox(this);
    m_cbGrainType->addItems(QStringList()
                                << tr("Cylindrical (BATES)")
                                << tr("Moonburner"));

    m_layControls->addWidget(new QLabel(tr("Core Shape")), 0, 0);
    m_layControls->addWidget(m_cbGrainType, 0, 1);

    //Propellant Selection Box
    m_cbPropellantType = new QComboBox(this);
    //add all propellants in DB to the combo box
    for (auto prop : m_app.GetAllPropellants())
    {
        m_cbPropellantType->addItem(prop.GetPropellantName());
    }
    m_layControls->addWidget(new QLabel(tr("Propellant Type")), 1, 0);
    m_layControls->addWidget(m_cbPropellantType, 1, 1);
    m_layControls->addWidget(m_btntModifyPropellant = new QToolButton, 1, 2);
    m_btntModifyPropellant->setText("...");

    //Grain Length
    m_layControls->addWidget(new QLabel(tr("Length")), 2, 0);
    m_layControls->addWidget(m_sbGrainLen = new QDoubleSpinBox(this), 2, 1);
    m_layControls->addWidget(m_unitsGrainLen = new LengthUnitsComboBox(this, m_sbGrainLen), 2, 2);
    m_unitsGrainLen->setLayoutDirection(Qt::LeftToRight);

    //Grain Diameter
    m_layControls->addWidget(new QLabel(tr("Diameter")), 3, 0);
    m_layControls->addWidget(m_sbGrainDia = new QDoubleSpinBox(this), 3, 1);
    m_layControls->addWidget(m_unitsGrainDia = new LengthUnitsComboBox(this, m_sbGrainDia), 3, 2);
    m_unitsGrainDia->setLayoutDirection(Qt::LeftToRight);

    //Inhibited Faces
    m_layControls->addWidget(new QLabel(tr("Inhibited Faces")), 255, 0);
    m_layControls->addWidget(m_sbGrainInhibit = new QSpinBox, 255, 1);
    m_sbGrainInhibit->setMaximum(2);

    setTabOrder(m_cbGrainType, m_cbPropellantType);
    setTabOrder(m_cbPropellantType, m_sbGrainLen);
    setTabOrder(m_sbGrainLen, m_sbGrainDia);
    setTabOrder(m_sbGrainDia, m_sbGrainInhibit);

    //special controls such as grain core diameter / star info / c slot wid / etc
    //default to BATES mode
    UpdateDesignControls(GrainType::BATES);

    // OK and Cancel buttons
    m_btnApply = new QPushButton(m_gbFrame);
    m_btnCancel = new QPushButton(m_gbFrame);
    m_btnApply->setText(m_isNewGrainWindow ? tr("Add") : tr("Apply"));
    m_btnCancel->setText(tr("Close"));

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

    if (!m_gfxGrain)
    {
        m_gfxGrain = std::make_unique<GrainGraphicsItem>(
            OpenBurnUtil::g_kGfxScaleFactor, m_GrainForGfx.get(),  false);
        m_GraphicsScene->addItem(m_gfxGrain.get());
    }
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(true);
    m_GraphicsView->setSizePolicy(sizePolicy2);
}
void GrainDialog::SetGrainValues()
{
    /*
    const double grainDia = m_unitsGrainDia->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches,
        m_sbGrainDia->value());
    const double grainLen = m_unitsGrainLen->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches,
        m_sbGrainLen->value());
    const int numInhibited = m_sbGrainInhibit->value();

    OpenBurnPropellant currentProp;
    const auto foundProp = std::find_if(m_Propellants->begin(), m_Propellants->end(),
                                  [&](const OpenBurnPropellant& prop) -> bool
                                  {
                                      return prop.GetPropellantName() == m_cbPropellantType->currentText();
                                  });
    if (foundProp != m_Propellants->end())
    {
        currentProp = *foundProp;
    }
    if (auto bates = dynamic_cast<CylindricalGrainDesign*>(m_GrainDesign.get()))
    {
        if (m_isNewGrainWindow)
        {
            auto grain = std::make_shared<CylindricalGrain>(
                grainDia,
                bates->GetCoreDiameter(),
                grainLen,
                currentProp,
                numInhibited);
            m_grainsToEdit.emplace_back(std::move(grain));
        }
        else
        {
            for (auto& i : m_grainsToEdit)
            {
                auto grain = dynamic_cast<CylindricalGrain*>(i.get());
                grain->SetDiameter(grainDia);
                grain->SetCoreDiameter(bates->GetCoreDiameter());
                grain->SetLength(grainLen);
                grain->SetInhibitedFaces(numInhibited);
                grain->SetPropellantType(currentProp);
            }
        }
    }
    if (auto moon = dynamic_cast<MoonGrainDesign*>(m_GrainDesign.get()))
    {
        if (m_isNewGrainWindow)
        {
            auto grain = std::make_shared<MoonGrain>(
                grainDia,
                moon->GetCoreDiameter(),
                grainLen,
                moon->GetCoreOffset(),
                currentProp,
                numInhibited);
            m_grainsToEdit.emplace_back(std::move(grain));
        }
        else
        {
            for (auto& i : m_grainsToEdit)
            {
                auto grain = dynamic_cast<MoonGrain*>(i.get());
                grain->SetDiameter(grainDia);
                grain->SetCoreDiameter(moon->GetCoreDiameter());
                grain->SetCoreOffset(moon->GetCoreOffset());
                grain->SetLength(grainLen);
                grain->SetInhibitedFaces(numInhibited);
                grain->SetPropellantType(currentProp);
            }
        }
    }
    */
}
void GrainDialog::SeedValues(OpenBurnGrain *seed)
{
    /*
    if (m_GlobalSettings)
    {
        m_unitsGrainDia->SetUnits(m_GlobalSettings->m_LengthUnits);
        m_unitsGrainLen->SetUnits(m_GlobalSettings->m_LengthUnits);
    }
    if (seed)
    {
        m_sbGrainLen->setValue(m_unitsGrainLen->GetCurrentUnits().ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            seed->GetLength()));
        m_sbGrainDia->setValue(m_unitsGrainLen->GetCurrentUnits().ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            seed->GetDiameter()));
        m_sbGrainInhibit->setValue(seed->GetInhibitedFaces());

        //find the seed's propellant type in the combo box
        auto items = m_cbPropellantType->model()->match(
            m_cbPropellantType->model()->index(0, 0),
            Qt::DisplayRole,
            QVariant::fromValue(seed->GetPropellantType().GetPropellantName()));

        if (!items.empty())
        {   //set the propellant type combo box to that idx
            m_cbPropellantType->setCurrentIndex(items.begin()->row());
        }
        if (dynamic_cast<CylindricalGrain*>(seed)){
            UpdateDesignControls(GrainType::BATES);
        }
        if (dynamic_cast<MoonGrain*>(seed)){
            UpdateDesignControls(GrainType::MOON);
        }
        UpdateGraphics();
    }
    */
}
void GrainDialog::UpdateDesignControls(GrainType type)
{
    /*
    m_GrainDesign.reset();
    m_cbGrainType->setCurrentIndex(static_cast<int>(type));
    //seed special values
    switch (type)
    {
        case GrainType::BATES:
        {
            m_GrainDesign = std::make_unique<CylindricalGrainDesign>(m_layControls, m_seedGrain, m_GlobalSettings);
            //special controls such as grain core diameter / star info / c slot wid / etc
            auto* coreDiaComboBox = static_cast<CylindricalGrainDesign*>(m_GrainDesign.get())->m_sbGrainCoreDia;
            setTabOrder(m_sbGrainDia, coreDiaComboBox);
            setTabOrder(coreDiaComboBox, m_sbGrainInhibit);
            break;
        }
        case GrainType::MOON:
        {
            m_GrainDesign = std::make_unique<MoonGrainDesign>(m_layControls, m_seedGrain, m_GlobalSettings);
            //special controls such as grain core diameter / star info / c slot wid / etc
            auto* coreDiaComboBox = static_cast<MoonGrainDesign*>(m_GrainDesign.get())->m_sbGrainCoreDia;
            auto* coreOffsetComboBox = static_cast<MoonGrainDesign*>(m_GrainDesign.get())->m_sbCoreOffset;
            setTabOrder(m_sbGrainDia, coreDiaComboBox);
            setTabOrder(coreDiaComboBox, coreOffsetComboBox);
            setTabOrder(coreOffsetComboBox, m_sbGrainInhibit);
        }
    }
    connect(m_GrainDesign.get(), &BaseGrainDesign::GrainDesignChanged,
            this, &GrainDialog::OnDesignUpdated);
    UpdateGraphics();
    */
}
void GrainDialog::UpdateGraphics()
{
    double grainDia = m_unitsGrainDia->GetCurrentUnits().ConvertTo(
        OpenBurnUnits::LengthUnits_T::inches,
        m_sbGrainDia->value());

    if (auto bates = dynamic_cast<CylindricalGrainDesign*>(m_GrainDesign.get()))
    {
        m_GrainForGfx.reset();
        m_GrainForGfx = std::make_unique<CylindricalGrain>();
        m_gfxGrain->UpdateGrain(m_GrainForGfx.get());

        m_GrainForGfx->SetDiameter(grainDia);
        static_cast<CylindricalGrain*>(m_GrainForGfx.get())->SetCoreDiameter(bates->GetCoreDiameter());
    }
    if (auto moon = dynamic_cast<MoonGrainDesign*>(m_GrainDesign.get()))
    {
        m_GrainForGfx.reset();
        m_GrainForGfx = std::make_unique<MoonGrain>();
        m_gfxGrain->UpdateGrain(m_GrainForGfx.get());

        m_GrainForGfx->SetDiameter(grainDia);
        static_cast<MoonGrain*>(m_GrainForGfx.get())->SetCoreDiameter(moon->GetCoreDiameter());
        static_cast<MoonGrain*>(m_GrainForGfx.get())->SetCoreOffset(moon->GetCoreOffset());
    }

    m_gfxGrain->setPos(0, 0);
    //set the display scene to the middle of the view plus a bit of padding on the sides
    m_GraphicsView->setSceneRect(m_gfxGrain->boundingRect());
    QRectF bounds = QRectF(m_gfxGrain->boundingRect().left(), m_gfxGrain->boundingRect().top(), 
        m_gfxGrain->boundingRect().width() + 50, m_gfxGrain->boundingRect().height() + 50);
    m_GraphicsView->fitInView(bounds, Qt::KeepAspectRatio);

    //update again just in case 
    m_GraphicsView->viewport()->repaint();
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
void GrainDialog::OnDesignUpdated()
{
    UpdateGraphics();
}
void GrainDialog::OnCancelButtonClicked()
{
    close();
}
void GrainDialog::OnApplyButtonClicked()
{
    //TODO: make this a small warning below the buttons or something rather than a msg box
    if (qFuzzyIsNull(m_sbGrainLen->value()))
    {
        QMessageBox::warning(this, tr("Warning!"),
            tr("Grain Length cannot be 0!\n"),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (qFuzzyIsNull(m_sbGrainDia->value()))
    {
        QMessageBox::warning(this, tr("Warning!"),
            tr("Grain Diameter cannot be 0!\n"),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    /*
    SetGrainValues();
    if (m_isNewGrainWindow)
    {
        emit GrainAdded(*m_grainsToEdit.begin());
        m_grainsToEdit.pop_back();
    }
    else
    {
		emit GrainsEdited(m_grainsToEdit, m_OriginalGrains);
    }
    */
}
void GrainDialog::OnGrainCoreTypeChanged(int index)
{
    GrainType type = static_cast<GrainType>(index);
    UpdateDesignControls(type);
}
