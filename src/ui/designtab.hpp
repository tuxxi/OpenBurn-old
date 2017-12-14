#pragma once

#include <QWidget>
#include <QLabel>
#include <QUndoStack>
#include <memory>

#include "src/ui/graphics/motorgraphicsitem.hpp"

#include "src/ui/dialogs/graindialog.hpp"
#include "src/ui/dialogs/nozzledialog.hpp"
#include "src/ui/widgets/graintablewidget.hpp"
#include "src/motorsim.hpp"
#include "src/settings.hpp"


class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(OpenBurnMotor* motor, 
		PropellantList* propellant, 
		OpenBurnSettings* settings, 
		QUndoStack* undoStack, 
		QWidget* parent = nullptr);
    ~DesignTab();
    void resizeEvent(QResizeEvent* event) override;

    void UpdateDesign();  
    void UpdateGraphics();

    void CreateNewGrain();
    void MoveGrains(bool up);
    void EditSelectedGrains();
    void DeleteSelectedGrains();

    void EditNozzle();
signals:
    //we selected (or un selected) something
    void SelectionChanged(bool selected);
public slots:
	//grain dialog
    void OnNewGrain(const GrainPtr& grain); 
	void OnGrainsModified(const GrainVector& newGrains, const GrainVector& originalGrains); 
	//nozzle dialog
	void OnNewNozzle(NozzlePtr& newNozzle);
    void OnNozzleUpdated(NozzlePtr& newNozzle, NozzlePtr& oldNozzle);

    //motor
    void OnDesignUpdated();
private slots:
    //buttons
    void OnNewGrainButtonClicked();
    void OnEditGrainButtonClicked();
    void OnNozzleButtonClicked();
    void OnDeleteGrainButtonClicked();
    void OnMoveGrainUpButtonClicked();
    void OnMoveGrainDownButtonClicked();

    //dialogs
    void OnGrainDialogClosed();
    void OnNozzleDialogClosed();

    //table view
    void OnTableSelectionChanged();
private:
    void SetSeed(OpenBurnGrain* grain);
    void ToggleDesignButtons(bool on);
    void SetupUI();

    //main widget: grain info table    
    GrainTableWidget* m_GrainTable;

    //commands that modify motor state get pushed in here. this is owned by MainWindow.
	QUndoStack* m_UndoStack;

    //grain design overview - static
    QLabel *m_lblMotorMajorDia, *m_lblMotorLen, *m_lblNumGrains, *m_lblPropellantMass, *m_lblVolumeLoading;
    //nozzle overview
    QLabel* m_lblNozzleThroatDia, *m_lblNozzleExitDia, *m_lblNozzleExpansionRatio;
    //motor general overview
    QLabel *m_lblKn, *m_lblPortThroatRatio;

    //grain settings
    std::unique_ptr<OpenBurnGrain> m_grainSeed; //stores settings to "seed" the dialog
    QPushButton *m_btnNewGrain, *m_btnDeleteGrain, *m_btnEditGrain;

    //controls
    QToolButton *m_btntMoveGrainUp, *m_btntMoveGrainDown;

    //nozzle settings
    QPushButton* m_btnNozzleSettings;

    //dialogs
	std::unique_ptr<GrainDialog> m_GrainDialog;
	std::unique_ptr<NozzleDialog> m_NozzleDialog;

    //gfx
    std::unique_ptr<MotorGraphicsItem> m_gfxMotor;
    QGraphicsView* m_MotorDisplayView;
    QGraphicsScene* m_MotorDisplayScene;

    //for reference pnly - these are all owned by MainWindow
    OpenBurnMotor* m_Motor; 
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
};
