#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSizePolicy>

#include "src/ui/graindialog.h"

class DesignTab : public QWidget
{
    Q_OBJECT
public:
    explicit DesignTab(QWidget *parent = nullptr);
    ~DesignTab();
private slots:
    void NewGrainButton_Clicked();
private:
    void SetupUI();
    QPushButton *newGrainButton;

    GrainDialog *dialog;
};