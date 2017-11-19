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
    void OnNewGrain(OpenBurnGrain* grain); //recieved from the grain dialog
    void OnGrainModified(OpenBurnGrain* grain); //recieved from the grain dialog
    void OnNozzleUpdated(OpenBurnNozzle* nozz);
private slots:
    void OnNewGrainButtonClicked();
    void OnEditGrainButtonClicked();
    void OnNozzleButtonClicked();
    void OnDeleteGrainButtonClicked();
    void OnMoveGrainUpButtonClicked();
    void OnMoveGrainDownButtonClicked();


    void OnGrainDialogClosed();
    void OnNozzleDialogClosed();
    void OnGrainTableCellClicked(int row, int column);
private:
    void SetSeed(OpenBurnGrain* grain);
    void SetupUI();
    void UpdateGraphics();  
    
    //grain design overview - static
    QLabel *m_lblMotorMajorDia, *m_lblMotorLen, *m_lblNumGrains, *m_lblPropellantMass, *m_lblVolumeLoading;
    //nozzle overview
    QLabel* m_lblNozzleThroatDia, *m_lblNozzleExitDia, *m_lblNozzleExpansionRatio;
    //motor general overview
    QLabel *m_lblKn, *m_lblPortThroatRatio;

    //simulated results - requires a sim to be run
    //for quick reference only- more detailed results are available on results tab
    //QLabel* m_maxPressureLabel, *m_motorDesignationLabel, *m_totalImpulseLabel;

    QPushButton* m_btnNozzleSettings;

    //grain settings
    OpenBurnGrain* m_grainSeed; //stores settings to "seed" the dialog
    QPushButton *m_btnNewGrain, *m_btnDeleteGrain, *m_btnEditGrain;

    //controls
    QToolButton *m_btntMoveGrainUp, *m_btntMoveGrainDown;

    GrainDialog* m_GrainDialog;
    NozzleDialog* m_NozzleDialog;
    GrainTableWidget* m_GrainTable;

    //gfx
    MotorGraphicsItem* m_gfxMotor;
    QGraphicsView* m_MotorDisplayView;
    QGraphicsScene* m_MotorDisplayScene;

    OpenBurnMotor* m_Motor; //The initial design
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
};
