#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include "src/grain.h"
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    GrainTableWidget(QWidget *parent);
    virtual void dropEvent(QDropEvent *event) override;
signals:
    void GrainPositionUpdated(int &oldPosition, int &newPosition);
};

