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
    ~NozzleDialog();
signals:
    void SIG_NozzleChanged(OpenBurnNozzle*);
private slots:
    bool apply();
    void accept();
    void closeDialog();
    void UpdateDesign();    

private:
    void SetupUI();
    void UpdateGraphics();    
    QFrame* m_frame;
    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_graphicsScene;

    OpenBurnDesignNozzle* m_NozzleDesign;
    OpenBurnNozzle* m_Nozzle;
    NozzleGraphicsItem* m_gfxNozzle;
    OpenBurnSettings* m_GlobalSettings;
    QPushButton *m_applyButton, *m_OKButton, *m_cancelButton;   
};
