#include "graintablewidget.h"

GrainTableWidget::GrainTableWidget(QWidget *parent) : QTableWidget(parent)
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
}
void GrainTableWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    for (int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, this->width()/columnCount());
    }    
}
void GrainTableWidget::AddNewGrain(OpenBurnGrain* grain)
{
    int numItems = rowCount();
    setRowCount(numItems+1);

    setItem(numItems, 0, new QTableWidgetItem(num(grain->GetLength())));
    setItem(numItems, 1, new QTableWidgetItem(num(grain->GetDiameter())));
    setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType()->GetPropellantName()));
    setItem(numItems, 4, new QTableWidgetItem(num(grain->GetInhibitedFaces())));

    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(grain))
    {
        setItem(numItems, 2, new QTableWidgetItem(num(bates->GetCoreDiameter())));
    }
}
void GrainTableWidget::ModifyGrain(OpenBurnGrain* grain, int idx)
{
    setItem(idx, 0, new QTableWidgetItem(num(grain->GetLength())));
    setItem(idx, 1, new QTableWidgetItem(num(grain->GetDiameter())));
    setItem(idx, 3, new QTableWidgetItem(grain->GetPropellantType()->GetPropellantName()));
    setItem(idx, 4, new QTableWidgetItem(num(grain->GetInhibitedFaces())));

    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(grain))
    {
        setItem(idx, 2, new QTableWidgetItem(num(bates->GetCoreDiameter())));
    }

}
void GrainTableWidget::DeleteGrain(int idx)
{
    removeRow(idx);
}
//thank you to honiahaka10 on stack overflow
//https://stackoverflow.com/a/41203632/8508673
void GrainTableWidget::dropEvent(QDropEvent* event)
{
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
        emit SIG_GrainPositionUpdated(oldRow, newRow);
    }
}
