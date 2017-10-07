#include "graindialog.h"
#include "ui_graindialog.h"
#include "grain.h"

GrainDialog::GrainDialog(QWidget *parent, bool newGrain) :
    QDialog(parent), m_isNewGrainWindow(newGrain),
    ui(new Ui::GrainDialog)
{
    ui->setupUi(this);
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
}

void GrainDialog::on_ApplyOrAddNewGrainButton_clicked()
{
    if (m_isNewGrainWindow)
    {
        BatesGrain *grain = new BatesGrain(
                    ui->grainCoreDiameterComboBox->value(),
                    ui->grainCoreDiameterComboBox->value(),
                    ui->grainLengthComboBox->value(),
                    OpenBurnPropellant());
    }
}
