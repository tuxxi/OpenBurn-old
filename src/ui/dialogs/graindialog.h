#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QGroupBox>

#include <QList>

#include "src/motor.h"
#include "src/settings.h"
#include "graindesigntypes.h"
#include "src/ui/graphics/graingraphicsitem.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(PropellantList* propellants,
        OpenBurnGrain* seedValues = nullptr, 
        OpenBurnSettings* settings = nullptr,
        const GrainVector& grains = GrainVector(),
        QWidget* parent = nullptr
        );
    ~GrainDialog() = default;
protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void GrainAdded(const std::shared_ptr<OpenBurnGrain>&);
    void GrainEdited(const std::shared_ptr<OpenBurnGrain>&);
private slots:
    void OnCancelButtonClicked();
    void OnApplyButtonClicked();
    void OnDesignUpdated();
private:
    void SetupUI(OpenBurnGrain* seed);
    void SetupGraphics();
    void UpdateGraphics();
    QGridLayout* m_layControls;
    QGroupBox* m_gbFrame;

    QGraphicsView *m_GraphicsView;
    QGraphicsScene *m_GraphicsScene;
    std::unique_ptr<GrainGraphicsItem> m_gfxGrain;
    QPushButton *m_btnApply, *m_btnCancel;

    OpenBurnDesignGrain* m_GrainDesign;
	GrainVector m_grainsToEdit;
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
    bool m_isNewGrainWindow;
};
