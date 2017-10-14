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

#include "src/grain.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget *parent = nullptr, bool newGrain = true);
    ~GrainDialog();

signals:
    void DialogClosed();
    void NewGrain(OpenBurnGrain *grain);

private slots:
    void on_cancelButton_clicked();
    void on_ApplyOrAddNewGrainButton_clicked();

private:
    void SetupUI();
    void SetupGraphicsView();

    QGraphicsView *graphicsView;
    QDoubleSpinBox *grainDiameterSpinBox, *grainCoreDiameterSpinBox, *grainLengthSpinBox;
    QSpinBox *grainInhibitedFacesSpinBox;

    QComboBox *propellantComboBox;
    QComboBox *grainLenUnitsComboBox, *grainDiaUnitsComboBox, *grainCoreDiaUnitsComboBox;

    QToolButton *modifyPropellantDatabase;

    QPushButton *ApplyOrAddNewGrainButton, *cancelButton;
    bool m_isNewGrainWindow;
};
