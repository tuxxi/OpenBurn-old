#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>

#include "src/util.h"
#include "src/nozzle.h"
#include "nozzledesigntypes.h"

class NozzleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NozzleDialog(QWidget* parent = nullptr);
    ~NozzleDialog();
signals:
    void SIG_NozzleChanged(OpenBurnNozzle*);
private slots:
    bool apply();
    void accept();
    void closeDialog();
private:
    void SetupUI();
    QFrame* m_frame;
    QGraphicsView *m_graphicsView;
    
    OpenBurnDesignNozzle* m_NozzleDesign;
    QPushButton *m_applyButton, *m_OKButton, *m_cancelButton;   
};
