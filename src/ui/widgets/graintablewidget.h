#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QTableView>
#include <QHeaderView>
#include <QList>

#include "src/motor.h"
#include "src/settings.h"
//This class manages the table of grains displayed in the design window.
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit GrainTableWidget(OpenBurnMotor* motor, OpenBurnSettings* settings, QWidget *parent = nullptr);
    ~GrainTableWidget();

    virtual void dropEvent(QDropEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    QList<int>GetSelectedGrainIndices();
    QList<OpenBurnGrain*>GetSelectedGrains();
    
    void move(bool up);
    QList<QTableWidgetItem*> takeRow(int row);
    void setRow(int row, const QList<QTableWidgetItem*>& rowItems);
public slots:
    void Update();

private:
    OpenBurnMotor* m_Motor;
    OpenBurnSettings* m_Settings;
};

