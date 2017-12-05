#pragma once

#include <QWidget>
#include <QLabel>

#include <memory>

#include "src/ui/graphics/motorgraphicsitem.h"

#include "src/ui/dialogs/graindialog.h"
#include "src/ui/dialogs/nozzledialog.h"
#include "src/ui/widgets/graintablewidget.h"
#include "src/motorsim.h"
#include "src/settings.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(OpenBurnMotor* motor, PropellantList* propellant, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~DesignTab();
    void UpdateDesign();  
    void UpdateGraphics();

    void resizeEvent(QResizeEvent* event) override;
public slots:
    void OnNewGrain(const std::shared_ptr<OpenBurnGrain>& grain); //recieved from the grain dialog
    void OnGrainModified(const std::shared_ptr<OpenBurnGrain>& grain); //recieved from the grain dialog
    void OnNozzleUpdated(OpenBurnNozzle* nozz);
    void OnDesignUpdated();
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
	void ToggleDesignButtons(bool on);
private:
    void SetSeed(OpenBurnGrain* grain);
    void SetupUI();
    
    //grain design overview - static
    QLabel *m_lblMotorMajorDia, *m_lblMotorLen, *m_lblNumGrains, *m_lblPropellantMass, *m_lblVolumeLoading;
    //nozzle overview
    QLabel* m_lblNozzleThroatDia, *m_lblNozzleExitDia, *m_lblNozzleExpansionRatio;
    //motor general overview
    QLabel *m_lblKn, *m_lblPortThroatRatio;

    //simulated results - requires a sim to be run
    //for quick reference only- more detailed results are available on results tab
    //QLabel* m_maxPressureLabel, *m_motorDesignationLabel, *m_totalImpulseLabel;
	GrainTableWidget* m_GrainTable;

    QPushButton* m_btnNozzleSettings;

    //grain settings
    std::unique_ptr<OpenBurnGrain> m_grainSeed; //stores settings to "seed" the dialog
    QPushButton *m_btnNewGrain, *m_btnDeleteGrain, *m_btnEditGrain;

    //controls
    QToolButton *m_btntMoveGrainUp, *m_btntMoveGrainDown;

	GrainDialog* m_GrainDialog;
	NozzleDialog* m_NozzleDialog;

    //gfx
    std::unique_ptr<MotorGraphicsItem> m_gfxMotor;
    QGraphicsView* m_MotorDisplayView;
    QGraphicsScene* m_MotorDisplayScene;

    OpenBurnMotor* m_Motor; //The initial design
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
};
