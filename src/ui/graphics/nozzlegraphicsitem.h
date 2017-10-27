#pragma once

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>

#include "src/nozzle.h"

//this class represents the 2D cross sectional view of a nozzle
class NozzleGraphicsItem : public QGraphicsObject
{
public:
    NozzleGraphicsItem(OpenBurnNozzle* nozzle, int scale_factor, double nozzle_height, bool crossSection = true, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
    void UpdateNozzle(OpenBurnNozzle* nozz);
    void UpdateHeight(double height);
private:
    QColor m_color;
    bool m_isCrossSectionView;
    int m_scaleFactor;
    double m_nozzleHeight, m_nozzleLength;
    OpenBurnNozzle* m_Nozzle;
};

