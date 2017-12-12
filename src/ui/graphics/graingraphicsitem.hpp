#pragma once
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>
#include "src/grain.hpp"

//this class represents the 2D cross sectional view of a grain
class GrainGraphicsItem : public QGraphicsObject
{
public:
    GrainGraphicsItem(OpenBurnGrain* grain, int scale_factor, bool crossSection = true, QGraphicsItem *parent = nullptr);
    ~GrainGraphicsItem() = default;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    const OpenBurnGrain* GetGrain() { return m_Grain; }
    void UpdateGrain(OpenBurnGrain* grain);
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    OpenBurnGrain* m_Grain;
    QColor m_Color;
    int m_ScaleFactor;
    bool m_isCrossSectionView;
};

