#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QGroupBox>

#include <QList>

#include "src/motor.hpp"
#include "src/settings.hpp"
#include "graindesigntypes.hpp"
#include "src/ui/graphics/graingraphicsitem.hpp"

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
	void closeEvent(QCloseEvent* event) override;
signals:
    void GrainAdded(const GrainPtr&);
    void GrainsEdited(const GrainVector& newGrains,
		const GrainVector& originalGrains);
	void DialogClosed();
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
	//because the grains are pointers and we explicitly modify their values via the dialog, 
	//we store a copy of the original grains for the undo action later on
	GrainVector m_OriginalGrains;
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
    bool m_isNewGrainWindow;
};
