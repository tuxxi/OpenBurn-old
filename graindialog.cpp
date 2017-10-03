#include "graindialog.h"
#include "ui_graindialog.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain),
    ui(new Ui::GrainDialog)
{
    ui->setupUi(this);
    if (m_isNewGrainWindow)
    {
        ui->ApplyOrAddNewGrainButton->setText(QApplication::translate("GrainConfirmNew", "Add", nullptr));
        setWindowTitle(QApplication::translate("GrainDialogNew", "Add New Grain", nullptr));
    }
    else
    {
        ui->ApplyOrAddNewGrainButton->setText(QApplication::translate("GrainConfirmModify", "Apply", nullptr));
        setWindowTitle(QApplication::translate("GrainDialogModify", "Modify Grain", nullptr));
    }
}

GrainDialog::~GrainDialog()
{
    delete ui;
}

void GrainDialog::on_cancelButton_clicked()
{
    this->hide();
}
