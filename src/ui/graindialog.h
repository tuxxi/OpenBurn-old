#pragma once

#include <QDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
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
    QVBoxLayout *masterVLayout;
    QFrame *frame;

    QVBoxLayout *controlsVLayout;

    QHBoxLayout *commandButtonsHLayout;    
    QHBoxLayout *propellantHLayout;    
    QHBoxLayout *grainLengthHLayout;
    QHBoxLayout *grainDiaHLayout;
    QHBoxLayout *grainCoreDiaHLayout;
    QHBoxLayout *grainFacesHLayout;

    QToolButton *modifyPropellantDatabase;    
    QComboBox *propellantComboBox;
    QLabel *label;

    QDoubleSpinBox *grainLengthComboBox;
    QLabel *label_2;
    QComboBox *grainLenUnitsComboBox;

    QDoubleSpinBox *grainDiameterComboBox;
    QLabel *label_3;
    QComboBox *grainDiaUnitsComboBox;

    QDoubleSpinBox *grainCoreDiameterComboBox;
    QLabel *label_4;
    QComboBox *grainCoreDiaUnitsComboBox;

    QSpinBox *grainInhibitedFacesComboBox;
    QLabel *label_5;

    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *ApplyOrAddNewGrainButton;
    QPushButton *cancelButton;
    
    QGraphicsView *graphicsView;

    bool m_isNewGrainWindow;
};
