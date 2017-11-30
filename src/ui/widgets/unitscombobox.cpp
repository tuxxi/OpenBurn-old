#include <QDebug>

#include "unitscombobox.h"

UnitsComboBoxBase::UnitsComboBoxBase(QWidget* parent)
    : QComboBox(parent)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(OnUnitsUpdated(int)));
}
void UnitsComboBoxBase::OnUnitsUpdated(int newIdx)
{
    Q_UNUSED(newIdx);
    emit UnitsChanged(this);
}
//template definitions in header file
