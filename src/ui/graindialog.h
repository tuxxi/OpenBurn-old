#pragma once

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
    explicit GrainDialog(QWidget *parent = nullptr, bool newGrain = true);
    ~GrainDialog();

signals:
    void SIG_DIALOG_NewGrain(OpenBurnGrain*);

private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
private:
    void SetupUI();
    void SetupGraphicsView();

    QGraphicsView *m_graphicsView;
    QDoubleSpinBox *m_grainDiameterSpinBox, *m_grainCoreDiameterSpinBox, *m_grainLengthSpinBox;
    QSpinBox *m_grainInhibitedFacesSpinBox;

    QComboBox *m_propellantComboBox;
    QComboBox *m_grainLenUnitsComboBox, *m_grainDiaUnitsComboBox, *m_grainCoreDiaUnitsComboBox;

    QToolButton *m_modifyPropellantDatabase;

    QPushButton *m_applyButton, *m_cancelButton;
    bool m_isNewGrainWindow;
};
