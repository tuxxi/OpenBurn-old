#pragma once

#include <QDialog>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QGridLayout>
#include <QDebug>

#include "src/grain.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget *parent = nullptr, bool newGrain = true);
    ~GrainDialog();

signals:
    void SIG_DialogClosed();
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
