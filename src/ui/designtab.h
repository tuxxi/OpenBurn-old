#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>

#include "src/ui/graindialog.h"
#include "src/ui/nozzledialog.h"
#include "src/ui/graintablewidget.h"
#include "../util.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(QWidget *parent = nullptr);
    ~DesignTab();

signals:
    void SIG_NewGrain(OpenBurnGrain *grain); //passes along to the main window which holds the sim object
public slots:
    void SLOT_GrainPositionUpdated(int oldPos, int newPos); 
    void SLOT_NewGrain(OpenBurnGrain *grain); //recieved from the grain dialog
    void SLOT_NozzleUpdated(OpenBurnNozzle* nozz);
private slots:
    void NewGrainButton_Clicked();
    void NozzleButton_Clicked();
    void SLOT_GrainDialogClosed();
    void SLOT_NozzDialogClosed();
    
private:
    void SetupUI();
    QPushButton *m_newGrainButton, *m_nozzleSettingsButton;

    GrainDialog *m_grainDialog;
    NozzleDialog *m_nozzleDialog;
    GrainTableWidget *m_grainsDisplay;
    QGraphicsView *m_grainCrossSection;
};