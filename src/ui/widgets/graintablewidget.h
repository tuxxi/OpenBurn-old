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
signals:
    void SIG_GrainPositionUpdated(int, int);
};

