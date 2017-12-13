#include "graintablewidget.hpp"
#include "src/units.hpp"
#include <QDebug>

GrainTableWidget::GrainTableWidget(OpenBurnMotor* motor, OpenBurnSettings* settings, QWidget *parent) 
    : QTableWidget(parent),
      m_Motor(motor),
      m_Settings(settings)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(5); //propellant, len, core dia, dia, inhibited face
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QStringList tableHeader = (QStringList() << 
        tr("Length") << 
        tr("Diameter") << 
        tr("Core Diameter") << 
        tr("Propellant") << 
        tr("Inhibited Faces"));

    setHorizontalHeaderLabels(tableHeader);
    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //disable drag drop for now until i can fox it
    //m_grainsDisplay->setDragEnabled(true);
    //m_grainsDisplay->setDragDropMode(QAbstractItemView::DragDrop);

    connect(m_Motor, &OpenBurnMotor::GrainAdded,
        this, &GrainTableWidget::OnGrainAdded);
    connect(m_Motor, &OpenBurnMotor::GrainRemoved,
        this, &GrainTableWidget::OnGrainRemoved);
    connect(m_Motor, &OpenBurnMotor::GrainsSwapped,
        this, &GrainTableWidget::OnGrainsSwapped);
    connect(m_Motor, &OpenBurnMotor::GrainUpdated,
        this, &GrainTableWidget::OnGrainUpdated);
}
void GrainTableWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    for (int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, this->width()/columnCount());
    }    
}
void GrainTableWidget::OnGrainRemoved(int idx)
{
    removeRow(idx);
}
void GrainTableWidget::OnGrainAdded(OpenBurnGrain* grain)
{
    const QString lengthUnits = m_Settings->m_LengthUnits.GetUnitSymbol();
    setRowCount(rowCount() + 1);

    const int idx = rowCount() - 1;
    setItem(idx, 0, new QTableWidgetItem(QString::number(
        m_Settings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            grain->GetLength()), 'f', 3) +
        " " + lengthUnits));

    setItem(idx, 1, new QTableWidgetItem(QString::number(
        m_Settings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            grain->GetDiameter()), 'f', 3) +
        " " + lengthUnits));

    setItem(idx, 3, new QTableWidgetItem(grain->GetPropellantType().GetPropellantName()));
    setItem(idx, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));

    if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(grain))
    {
        setItem(idx, 2, new QTableWidgetItem(QString::number(
            m_Settings->m_LengthUnits.ConvertFrom(
                OpenBurnUnits::LengthUnits_T::inches,
                bates->GetCoreDiameter()), 'f', 3) +
            " " + lengthUnits));
    }
}

void GrainTableWidget::OnGrainUpdated(OpenBurnGrain* grain, int idx)
{
    const QString lengthUnits = m_Settings->m_LengthUnits.GetUnitSymbol();

    item(idx, 0)->setText(QString::number(
        m_Settings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            grain->GetLength()), 'f', 3) +
        " " + lengthUnits);

    item(idx, 1)->setText(QString::number(
        m_Settings->m_LengthUnits.ConvertFrom(
            OpenBurnUnits::LengthUnits_T::inches,
            grain->GetDiameter()), 'f', 3) +
        " " + lengthUnits);

    item(idx, 3)->setText(grain->GetPropellantType().GetPropellantName());
    item(idx, 4)->setText(QString::number(grain->GetInhibitedFaces()));

    if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(grain))
    {
        item(idx, 2)->setText(QString::number(
            m_Settings->m_LengthUnits.ConvertFrom(
                OpenBurnUnits::LengthUnits_T::inches,
                bates->GetCoreDiameter()), 'f', 3) +
            " " + lengthUnits);
    }
}

void GrainTableWidget::OnGrainsSwapped(int idx1, int idx2)
{
    auto row = TakeRow(idx1);
    auto row2 = TakeRow(idx2);
    SetRow(idx1, row2);
    SetRow(idx2, row);
    selectRow(idx2);
}

QList<int> GrainTableWidget::GetSelectedGrainIndices()
{
    QList<int> selectedList;
    int counter = 0;    
    for (auto* i : selectedItems())
    {
        int idx = row(i);
        counter++;
        if (idx != -1 && counter % columnCount() == 0)
        {
            selectedList.push_back(idx);
        }  
    }
    return selectedList;
}
GrainVector GrainTableWidget::GetSelectedGrains()
{
    GrainVector selectedList;
    int counter = 0;    
    for (auto* i : selectedItems())
    {
        int idx = row(i);
        counter++;
        if (idx != -1 && counter % columnCount() == 0)
        {
            selectedList.emplace_back(m_Motor->GetGrains()[idx]);
        }  
    }
    return selectedList;
}
// takes and returns the whole row
QList<QTableWidgetItem*> GrainTableWidget::TakeRow(int row)
{
    QList<QTableWidgetItem*> rowItems;
    for (int col = 0; col < columnCount(); ++col)
    {
        rowItems << takeItem(row, col);
    }
    return rowItems;
}
// sets the whole row
void GrainTableWidget::SetRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
    for (int col = 0; col < columnCount(); ++col)
    {
        setItem(row, col, rowItems.at(col));
    }
}
//thank you to honiahaka10 on stack overflow
//https://stackoverflow.com/a/41203632/8508673
void GrainTableWidget::dropEvent(QDropEvent* event)
{
    Q_UNUSED(event);
    /*
    if(event->source() == this)
    {
        int newRow = this->indexAt(event->pos()).row();
        QTableWidgetItem *selectedItem;
        QList<QTableWidgetItem*> selectedItems = this->selectedItems();
        if(newRow == -1)
            newRow = this->rowCount();
        for(int i = 0; i < selectedItems.length()/this->columnCount(); i++)
        {
            this->insertRow(newRow);
        }
        int currentOldRow = -1;
        int currentNewRow = newRow-1;
        QList<int> deleteRows;
        foreach(selectedItem, selectedItems)
        {
            int column = selectedItem->column();
            if(selectedItem->row() != currentOldRow)
            {
                currentOldRow = selectedItem->row();
                deleteRows.append(currentOldRow);
                currentNewRow++;
            }
            this->takeItem(currentOldRow, column);
            this->setItem(currentNewRow, column, selectedItem);
        }

        for(int i = deleteRows.count()-1; i>=0; i--)
        {
            this->removeRow(deleteRows.at(i));
        }
        int oldRow = (currentOldRow);
        newRow++;
    }
    */
}
