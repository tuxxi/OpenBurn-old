#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QGroupBox>

#include "src/grain.h"
#include "src/util.h"
#include "graindesigntypes.h"
#include "src/ui/graphics/graingraphicsitem.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget* parent = nullptr, OpenBurnGrain* grain = nullptr, bool newGrain = true);
    virtual ~GrainDialog();

signals:
    void SIG_DIALOG_NewGrain(OpenBurnGrain*);
    void SIG_DIALOG_EditGrain(OpenBurnGrain*);
private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void UpdateDesign();    
    void RefreshUI(GRAINTYPE type);    
private:
    void SetupUI(OpenBurnGrain* seed);
    void SetupGraphics();
    void UpdateGraphics();
    void resizeEvent(QResizeEvent* event) override;
    QGridLayout* m_controlsLayout;
    QGroupBox* m_frame;

    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_graphicsScene;
    GrainGraphicsItem *m_gfxGrain;
    QPushButton *m_applyButton, *m_cancelButton;

    OpenBurnDesignGrain* m_GrainDesign;
    OpenBurnGrain* m_Grain;
    GRAINTYPE m_GrainType;
    bool m_isNewGrainWindow;
};
