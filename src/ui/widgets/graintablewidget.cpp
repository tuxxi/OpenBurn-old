#include "graintablewidget.h"
#include "src/units.h"

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
}
void GrainTableWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    for (int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, this->width()/columnCount());
    }    
}
void GrainTableWidget::OnMotorUpdated()
{
    const QString lengthUnits = m_Settings->m_LengthUnits.GetUnitSymbol();
    setRowCount(0);
    for (auto& grain : m_Motor->GetGrains())
    {
        int numItems = rowCount();
        setRowCount(numItems+1);
    
        setItem(numItems, 0, new QTableWidgetItem(QString::number(
            m_Settings->m_LengthUnits.ConvertFrom(
                OpenBurnUnits::LengthUnits_T::inches,
                grain->GetLength()), 'f', 3) +
            " " +
            lengthUnits));
        
        setItem(numItems, 1, new QTableWidgetItem(QString::number(
            m_Settings->m_LengthUnits.ConvertFrom(
                OpenBurnUnits::LengthUnits_T::inches,
                grain->GetDiameter()), 'f', 3) +
            " " +
            lengthUnits));

        
        setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType().GetPropellantName()));
        setItem(numItems, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));
    
        if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(grain.get()))
        {
            setItem(numItems, 2, new QTableWidgetItem(QString::number(
                m_Settings->m_LengthUnits.ConvertFrom(
                OpenBurnUnits::LengthUnits_T::inches,
                bates->GetCoreDiameter()), 'f', 3) +
            " " +
            lengthUnits));
        }    
    }
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
QList<OpenBurnGrain*> GrainTableWidget::GetSelectedGrains()
{
    QList<OpenBurnGrain*> selectedList;
    int counter = 0;    
    for (auto* i : selectedItems())
    {
        int idx = row(i);
        counter++;
        if (idx != -1 && counter % columnCount() == 0)
        {
            selectedList.push_back(m_Motor->GetGrains()[idx].get());
        }  
    }
    return selectedList;
}
void GrainTableWidget::Move(bool up)
{
    Q_ASSERT(selectedItems().count() > 0);
    const int sourceRow = row(selectedItems().at(0));
    const int destRow = (up ? sourceRow-1 : sourceRow+1);
    Q_ASSERT(destRow >= 0 && destRow < rowCount());
 
    // take whole rows
    QList<QTableWidgetItem*> sourceItems = TakeRow(sourceRow);
    QList<QTableWidgetItem*> destItems = TakeRow(destRow);
 
    // set back in reverse order
    SetRow(sourceRow, destItems);
    SetRow(destRow, sourceItems);
    selectRow(destRow);
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
