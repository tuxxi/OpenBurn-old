#pragma once

#include <QDialog>

namespace Ui {
class GrainDialog;
}

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(QWidget *parent = nullptr, bool newGrain = true);
    ~GrainDialog();

private slots:
    void on_cancelButton_clicked();

private:
    Ui::GrainDialog *ui;
    bool m_isNewGrainWindow;
};
