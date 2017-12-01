#pragma once
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <vector>
#include "src/ui/graphics/graingraphicsitem.h"
#include "src/ui/graphics/nozzlegraphicsitem.h"

#include "src/grain.h"
#include "src/nozzle.h"

//this class represents the 2D cross sectional view of the whole motor
class MotorGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MotorGraphicsItem(int scale_factor, bool allowSlices = false, QGraphicsItem *parent = nullptr);
    ~MotorGraphicsItem() = default;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    void SetGrains(const std::vector<OpenBurnGrain*>& grains);
    void UpdateGrains(const std::vector<OpenBurnGrain*>& grains);
    void SetNozzle(OpenBurnNozzle* nozzle);
    void RemoveGrain(int index);
    void RemoveAllGrains();
    void SetScaleFactor(int scale);
    int GetScaleFactor() { return m_ScaleFactor; }
    double GetCurrentXPosSlice();
    void SetXPos(double xpos);
signals:
    void MotorXPosSliceUpdated(double slice);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void CalculateMotorHeight();
    int m_ScaleFactor;
    std::vector<GrainGraphicsItem*> m_gfxGrains;
    NozzleGraphicsItem* m_gfxNozzle;
    double m_MotorLen, m_MotorHeight; //in pixnels
    double m_currentSliceLocation;
    bool m_bAllowSlice;
};

