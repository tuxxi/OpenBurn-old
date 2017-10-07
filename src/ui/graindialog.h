#pragma once

#include <QDialog>
#include "src/grain.h"

namespace Ui {
class GrainDialog;
}

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget *parent = nullptr, bool newGrain = true);
    ~GrainDialog();

signals:
    void DialogClosed();
    void NewGrain(OpenBurnGrain *grain);

private slots:
    void on_cancelButton_clicked();

    void on_ApplyOrAddNewGrainButton_clicked();

private:
    Ui::GrainDialog *ui;
    bool m_isNewGrainWindow;
};
