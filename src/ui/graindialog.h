#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>

#include "src/grain.h"
#include "../util.h"
#include "graindesigntypes.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget* parent = nullptr, OpenBurnGrain* grain = nullptr);
    virtual ~GrainDialog();

signals:
    void SIG_DIALOG_NewGrain(OpenBurnGrain*);
    void SIG_DIALOG_EditGrain(OpenBurnGrain*);
private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void SLOT_GrainType_Changed(GRAINTYPE type);
private:
    void SetupUI();
    void RefreshUI();
    QGridLayout* m_controlsLayout;
    QFrame* m_frame;

    QGraphicsView *m_graphicsView;
    QPushButton *m_applyButton, *m_cancelButton;

    OpenBurnGrainDesign* m_GrainDesign;
    OpenBurnGrain* m_Grain;
    GRAINTYPE m_GrainType;
    bool m_isNewGrainWindow;
};
