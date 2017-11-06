#include "graintablewidget.h"

GrainTableWidget::GrainTableWidget(OpenBurnMotor* motor, QWidget *parent) 
    : QTableWidget(parent), m_Motor(motor)
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
GrainTableWidget::~GrainTableWidget() {} 

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

    setItem(numItems, 0, new QTableWidgetItem(QString::number(grain->GetLength())));
    setItem(numItems, 1, new QTableWidgetItem(QString::number(grain->GetDiameter())));
    setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType()->GetPropellantName()));
    setItem(numItems, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));

    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(grain))
    {
        setItem(numItems, 2, new QTableWidgetItem(QString::number(bates->GetCoreDiameter())));
    }
    m_Motor->AddGrain(grain);
}
void GrainTableWidget::ModifyGrain(OpenBurnGrain* grain, int idx)
{
    setItem(idx, 0, new QTableWidgetItem(QString::number(grain->GetLength())));
    setItem(idx, 1, new QTableWidgetItem(QString::number(grain->GetDiameter())));
    setItem(idx, 3, new QTableWidgetItem(grain->GetPropellantType()->GetPropellantName()));
    setItem(idx, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));

    if (BatesGrain* bates = dynamic_cast<BatesGrain*>(grain))
    {
        setItem(idx, 2, new QTableWidgetItem(QString::number(bates->GetCoreDiameter())));
    }
    m_Motor->UpdateGrain(grain, idx);
}
QList<OpenBurnGrain*> GrainTableWidget::GetSelectedGrains()
{
    QList<OpenBurnGrain*> selectedList;
    int counter = 0;
    for (auto* i : selectedItems())
    {
        int idx = row(i);        
         //selectedItems is each cell in the table so we need to only add the row once
        counter++;
        if (idx != -1 && counter % columnCount() == 0)
        {
            selectedList.push_back(m_Motor->GetGrains()[idx]);
        }
    }
    return selectedList;
}
void GrainTableWidget::DeleteGrain(int idx)
{
    removeRow(idx);
    m_Motor->RemoveGrain(idx);
}
void GrainTableWidget::DeleteSelectedGrains()
{
    int counter = 0;
    for (auto* i : selectedItems())
    {
        int idx = row(i);        
         //selectedItems is each cell in the table so we need to only modify the row once
        counter++;
        if (idx != -1 && counter % columnCount() == 0)
        {
            m_Motor->RemoveGrain(idx);
            removeRow(idx);
        }
    }
}
void GrainTableWidget::move(bool up)
{
    Q_ASSERT(selectedItems().count() > 0);
    const int sourceRow = row(selectedItems().at(0));
    const int destRow = (up ? sourceRow-1 : sourceRow+1);
    Q_ASSERT(destRow >= 0 && destRow < rowCount());
 
    // take whole rows
    QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
    QList<QTableWidgetItem*> destItems = takeRow(destRow);
 
    // set back in reverse order
    setRow(sourceRow, destItems);
    setRow(destRow, sourceItems);
    selectRow(destRow);
    m_Motor->SwapGrains(sourceRow, destRow);
}
// takes and returns the whole row
QList<QTableWidgetItem*> GrainTableWidget::takeRow(int row)
{
    QList<QTableWidgetItem*> rowItems;
    for (int col = 0; col < columnCount(); ++col)
    {
        rowItems << takeItem(row, col);
    }
    return rowItems;
}
// sets the whole row
void GrainTableWidget::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
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
        m_Motor->SwapGrains(oldRow, newRow);
    }
}
