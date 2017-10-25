#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>

#include "src/ui/graindialog.h"
#include "src/ui/nozzledialog.h"
#include "src/ui/graintablewidget.h"
#include "src/util.h"
#include "src/motorsim.h"
#include "src/ui/graphics/motorgraphicsitem.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(QWidget *parent = nullptr);
    ~DesignTab();
    void resizeEvent(QResizeEvent* event) override;
signals:
    void SIG_NewGrain(OpenBurnGrain *grain); //passes along to the main window which holds the sim object
public slots:
    void SLOT_GrainPositionUpdated(int oldPos, int newPos); 
    void SLOT_NewGrain(OpenBurnGrain *grain); //recieved from the grain dialog
    void SLOT_NozzleUpdated(OpenBurnNozzle* nozz);
private slots:
    void NewGrainButton_Clicked();
    void NozzleButton_Clicked();
    void DeleteGrainButton_Clicked();
    void SLOT_GrainDialogClosed();
    void SLOT_NozzDialogClosed();
    void SLOT_grainTable_cellClicked(int row, int column);
    
private:
    void SetupUI();
    void UpdateGraphicsScene();
    
    OpenBurnNozzle* m_seed_nozzle;    
    QPushButton *m_nozzleSettingsButton;

    //grain settings
    OpenBurnGrain* m_seed_grain; //stores settings to "seed" the dialog
    QPushButton *m_newGrainButton, *m_deleteGrainButton;

    GrainDialog *m_grainDialog;
    NozzleDialog *m_nozzleDialog;
    GrainTableWidget *m_grainsDisplay;

    MotorGraphicsItem *m_motorObject;
    QGraphicsView *m_motorDisplayView;
    QGraphicsScene* m_motorDisplayScene;

    QGraphicsTextItem* knDisplay;

    MotorSim* m_sim;
};