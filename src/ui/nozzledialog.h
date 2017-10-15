#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>

#include "../util.h"
#include "../nozzle.h"
class NozzleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NozzleDialog(QWidget* parent = nullptr);
    ~NozzleDialog();
signals:
    void SIG_NozzleChanged(OpenBurnNozzle*);
private slots:
    void apply();
    void accept();
    void closeDialog();
private:
    void SetupUI();
    QPushButton *m_applyButton, *m_OKButton, *m_cancelButton;   
    QDoubleSpinBox *m_throatDiaSpinBox, *m_exitDiaSpinBox, *m_halfAngleSpinBox, *m_throatLenSpinBox;
    QComboBox *m_throatDiaUnits, *m_exitDiaUnits, *m_halfAngleUnits, *m_throadLenUnits;
};
