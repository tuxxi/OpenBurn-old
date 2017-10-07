#include "graindialog.h"
#include "ui_graindialog.h"
#include "src/grain.h"
#include "mainwindow.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain),
    ui(new Ui::GrainDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    if (m_isNewGrainWindow)
    {
        ui->ApplyOrAddNewGrainButton->setText(QApplication::translate("GrainConfirmNew", "Add"));
        setWindowTitle(QApplication::translate("GrainDialogNew", "Add New Grain"));
    }
    else
    {
        ui->ApplyOrAddNewGrainButton->setText(QApplication::translate("GrainConfirmModify", "Apply"));
        setWindowTitle(QApplication::translate("GrainDialogModify", "Modify Grain"));
    }

    QStringList units = (QStringList() <<
        QApplication::translate("inches", "Inches (in)") <<
        QApplication::translate("millimeters", "Millimeters (mm)") <<
        QApplication::translate("centimeters", "Centimeters (cm)") <<
        QApplication::translate("feet", "Feet (ft)") <<
        QApplication::translate("meters", "Meters (m)"));

    ui->grainDiaUnitsComboBox->addItems(units);
    ui->grainCoreDiaUnitsComboBox->addItems(units);
    ui->grainLenUnitsComboBox->addItems(units);

}

GrainDialog::~GrainDialog()
{
    delete ui;
}
void GrainDialog::on_cancelButton_clicked()
{
    close();
    emit DialogClosed();
}

void GrainDialog::on_ApplyOrAddNewGrainButton_clicked()
{
    if (m_isNewGrainWindow)
    {
        BatesGrain *grain = new BatesGrain(
                    ui->grainDiameterComboBox->value(),
                    ui->grainCoreDiameterComboBox->value(),
                    ui->grainLengthComboBox->value(),
                    *(new OpenBurnPropellant()));
        emit NewGrain(grain);
    }
    else //Editing grain
    {

    }
}
