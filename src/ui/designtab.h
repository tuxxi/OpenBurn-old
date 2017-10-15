#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QGraphicsView>
#include <QDebug>

#include "src/ui/graindialog.h"
#include "src/ui/graintablewidget.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(QWidget *parent = nullptr);
    ~DesignTab();
signals:
    void SIG_NewGrain(OpenBurnGrain *grain); //passes along to the main window which holds the sim object
public slots:
    void SLOT_DialogClosed();
    void SLOT_NewGrain(OpenBurnGrain *grain); //recieved from the grain dialog

private slots:
    void NewGrainButton_Clicked();
private:
    void SetupUI();
    QPushButton *m_newGrainButton;

    GrainDialog *m_grainDialog;
    GrainTableWidget *m_grainsDisplay;
    QGraphicsView *m_grainCrossSection;
};