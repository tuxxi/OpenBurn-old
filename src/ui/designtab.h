#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFrame>
#include <QVBoxLayout>
#include <QGroupBox>

#include "src/ui/graindialog.h"
#include "src/ui/graintablewidget.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(QWidget *parent = nullptr);
    ~DesignTab();

public slots:
    void DialogClosed();
    void NewGrain(OpenBurnGrain *grain);
private slots:
    void NewGrainButton_Clicked();
private:
    void SetupUI();
    QPushButton *m_newGrainButton;

    GrainDialog *m_grainDialog;
    GrainTableWidget *m_grainsDisplay;
};