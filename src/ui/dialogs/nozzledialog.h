#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>

#include "src/nozzle.h"
#include "src/settings.h"
#include "nozzledesigntypes.h"
#include "src/ui/graphics/nozzlegraphicsitem.h"

class NozzleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NozzleDialog(QWidget* parent = nullptr, OpenBurnNozzle* seed = nullptr, OpenBurnSettings* settings = nullptr);
    ~NozzleDialog() = default;
signals:
    void NozzleChanged(OpenBurnNozzle*);
private slots:
    virtual void accept() override;

    bool OnApplyButtonClicked();
    void OnCloseButtonClicked();
    void OnDesignUpdated();

private:
    void SetupUI();
    void UpdateGraphics();    
    QFrame* m_Frame;

    QPushButton *m_btnApply, *m_btnOK, *m_btnClose;

    QGraphicsView *m_NozzleGraphicsView;
    QGraphicsScene *m_NozzleGraphicsScene;

    OpenBurnDesignNozzle* m_NozzleDesign;
    OpenBurnNozzle* m_Nozzle;
    NozzleGraphicsItem* m_gfxNozzle;
    OpenBurnSettings* m_GlobalSettings;
};
