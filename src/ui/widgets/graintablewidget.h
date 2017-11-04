#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QTableView>
#include <QHeaderView>

#include "src/grain.h"
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    GrainTableWidget(QWidget *parent);
    virtual void dropEvent(QDropEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    void AddNewGrain(OpenBurnGrain* grain);
    void ModifyGrain(OpenBurnGrain* grain, int idx);    
    void DeleteGrain(int idx);
    void DeleteSelectedGrains();
signals:
    void SIG_GrainPositionUpdated(int, int);
};

