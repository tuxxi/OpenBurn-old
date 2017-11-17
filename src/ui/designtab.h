#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QLabel>
#include <QToolButton>

#include "src/ui/graphics/motorgraphicsitem.h"

#include "src/ui/dialogs/graindialog.h"
#include "src/ui/dialogs/nozzledialog.h"
#include "src/ui/widgets/graintablewidget.h"
#include "src/util.h"
#include "src/motor.h"
#include "src/motorsim.h"
#include "src/settings.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(OpenBurnMotor* motor, PropellantList* propellant, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~DesignTab();
    void resizeEvent(QResizeEvent* event) override;
    void UpdateDesign();  
public slots:
    void SLOT_NewGrain(OpenBurnGrain* grain); //recieved from the grain dialog
    void SLOT_ModifyGrain(OpenBurnGrain* grain); //recieved from the grain dialog
    void SLOT_NozzleUpdated(OpenBurnNozzle* nozz);
private slots:
    void NewGrainButton_Clicked();
    void EditGrainButton_Clicked();
    void NozzleButton_Clicked();
    void DeleteGrainButton_Clicked();
    void MoveGrainUpButton_Clicked();
    void MoveGrainDownButton_Clicked();


    void SLOT_GrainDialogClosed();
    void SLOT_NozzDialogClosed();
    void SLOT_grainTable_cellClicked(int row, int column);
private:
    void SetSeed(OpenBurnGrain* grain);
    void SetupUI();
    void UpdateGraphics();  
    
    //design overview - static
    QLabel *m_motorMajorDiaLabel, *m_motorLenLabel, *m_numGrainsLabel, *m_propellantMassLabel, *m_VolumeLoadingLabel;

    QLabel* m_nozzleDiaLabel, *m_nozzleExitLabel, *m_expansionRatioLabel; 

    QLabel *m_knLabel, *m_portThroatRatioLabel;
    //simulated results - requires a sim to be run
    //for quick reference only- more detailed results are available on results tab
    //QLabel* m_maxPressureLabel, *m_motorDesignationLabel, *m_totalImpulseLabel;

    QPushButton* m_nozzleSettingsButton;

    //grain settings
    OpenBurnGrain* m_seed_grain; //stores settings to "seed" the dialog
    QPushButton *m_newGrainButton, *m_deleteGrainButton, *m_editGrainButton;

    //controls
    QToolButton *m_moveGrainUp, *m_moveGrainDown, *m_moveGrainToTop, *m_moveGrainToBottom;

    GrainDialog* m_grainDialog;
    NozzleDialog* m_nozzleDialog;
    GrainTableWidget* m_grainTable;

    MotorGraphicsItem* m_motorObject;
    QGraphicsView* m_motorDisplayView;
    QGraphicsScene* m_motorDisplayScene;
    OpenBurnMotor* m_Motor; //The initial design  
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
};
