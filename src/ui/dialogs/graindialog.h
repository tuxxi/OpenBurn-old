#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QGroupBox>
#include <QList>

#include "src/grain.h"
#include "src/settings.h"
#include "graindesigntypes.h"
#include "src/ui/graphics/graingraphicsitem.h"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    //By default, a new window with no seed values and no grain ptrs to edit exists.
    //This sets the dialog in "new grain" mode where a new grain pointer is allocted 
    //every time the "apply/add" button is clicked.
    //We can also "seed" the dialog with values from the previous editing session, but if the QList of grain ptrs is
    //empty, it will still be in "new grain" mode. For editing, supply a QList of OpenBurnGrain pointers and the dialog will populate
    //them all of all of them with the dialog's settings.
    explicit GrainDialog(PropellantList* propellants,
        OpenBurnGrain* seedValues = nullptr, 
        OpenBurnSettings* settings = nullptr,
        QList<OpenBurnGrain*>m_GrainsToEdit = QList<OpenBurnGrain*>(),
        QWidget* parent = nullptr
        );
    virtual ~GrainDialog();

signals:
    void SIG_DIALOG_NewGrain(OpenBurnGrain*);
    void SIG_DIALOG_EditGrain(OpenBurnGrain*);
private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void UpdateDesign();    
    void RefreshUI(GRAINTYPE type);    
private:
    void SetupUI(OpenBurnGrain* seed);
    void SetupGraphics();
    void UpdateGraphics();
    void resizeEvent(QResizeEvent* event) override;
    QGridLayout* m_controlsLayout;
    QGroupBox* m_frame;

    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_graphicsScene;
    GrainGraphicsItem *m_gfxGrain;
    QPushButton *m_applyButton, *m_cancelButton;

    OpenBurnDesignGrain* m_GrainDesign;
    QList<OpenBurnGrain*>m_GrainsToEdit;
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
    bool m_isNewGrainWindow;
};
