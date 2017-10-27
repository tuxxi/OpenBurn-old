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

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget* parent = nullptr, OpenBurnGrain* grain = nullptr);
    ~GrainDialog();

signals:
    void SIG_DIALOG_NewGrain(OpenBurnGrain*);
    void SIG_DIALOG_EditGrain(OpenBurnGrain*);
private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void on_grainType_changed(const QString& text);
private:
    void SetupUI();
    void RefreshUI();
    void SetupGraphicsView();
    void SeedValues();
    QGridLayout* controlsLayout;
    QFrame* frame;


    QGraphicsView *m_graphicsView;
    QDoubleSpinBox *m_grainDiameterSpinBox, *m_grainCoreDiameterSpinBox, *m_grainLengthSpinBox;
    QSpinBox *m_grainInhibitedFacesSpinBox;

    QComboBox *m_grainTypeComboBox;
    QComboBox *m_propellantComboBox;
    QComboBox *m_grainLenUnitsComboBox, *m_grainDiaUnitsComboBox, *m_grainCoreDiaUnitsComboBox;

    QToolButton *m_modifyPropellantDatabase;

    QPushButton *m_applyButton, *m_cancelButton;

    OpenBurnGrain* m_Grain;
    GRAINTYPE m_GrainType;
    bool m_isNewGrainWindow;
};
