#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QTableView>
#include <QHeaderView>
#include <QList>

#include "src/motor.h"

//This class manages the table of grains displayed in the design window.
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit GrainTableWidget(OpenBurnMotor* motor, QWidget *parent = nullptr);
    ~GrainTableWidget();


    virtual void dropEvent(QDropEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    void AddNewGrain(OpenBurnGrain* grain);
    void ModifyGrain(OpenBurnGrain* grain, int idx);

    QList<OpenBurnGrain*>GetSelectedGrains();
    void DeleteGrain(int idx);
    void DeleteSelectedGrains();

    void move(bool up);
    QList<QTableWidgetItem*> takeRow(int row);
    void setRow(int row, const QList<QTableWidgetItem*>& rowItems);
private:
    OpenBurnMotor* m_Motor;
};

