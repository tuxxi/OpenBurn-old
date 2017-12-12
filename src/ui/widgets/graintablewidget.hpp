#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QTableView>
#include <QHeaderView>
#include <QList>

#include "src/motor.hpp"
#include "src/settings.hpp"

//This class manages the table of grains displayed in the design window.

//TODO: maybe move to tree view?
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit GrainTableWidget(OpenBurnMotor* motor, OpenBurnSettings* settings, QWidget *parent = nullptr);
    ~GrainTableWidget() = default;

    QList<int>GetSelectedGrainIndices();
    GrainVector GetSelectedGrains();

public slots:
    void OnMotorUpdated();

protected:
    virtual void dropEvent(QDropEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    //TODO: use these and get rid of the ugly hack I was doing before :^)
    void Move(bool up);
    QList<QTableWidgetItem*> TakeRow(int row);
    void SetRow(int row, const QList<QTableWidgetItem*>& rowItems);

    OpenBurnMotor* m_Motor;
    OpenBurnSettings* m_Settings;
};

