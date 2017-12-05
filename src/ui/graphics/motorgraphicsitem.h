#pragma once
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <vector>
#include <memory>

#include "src/ui/graphics/graingraphicsitem.h"
#include "src/ui/graphics/nozzlegraphicsitem.h"

#include "src/grain.h"
#include "src/nozzle.h"
#include "src/motor.h"

//this class represents the 2D cross sectional view of the whole motor
class MotorGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MotorGraphicsItem(int scale_factor, OpenBurnMotor* motor, bool allowSlices = false, QGraphicsItem *parent = nullptr);
    ~MotorGraphicsItem() = default;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	void SetGrains(const std::vector<OpenBurnGrain*>& grains);
	void BurnGrains(const std::vector<OpenBurnGrain*>& grains);
    void UpdateGrains(const std::vector<OpenBurnGrain*>& grains);
	void RemoveAllGrains();

    void SetNozzle(OpenBurnNozzle* nozzle);
	void HideNozzle();
    void SetScaleFactor(int scale);
    int GetScaleFactor() { return m_ScaleFactor; }
    double GetCurrentXPosSlice();
    void SetXPos(double xpos);

public slots:
	void AddGrain(OpenBurnGrain* grain);
	void RemoveGrain(int idx);
	void OnMotorUpdated();
signals:
    void MotorXPosSliceUpdated(double slice);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void CalculateMotorHeight();
    int m_ScaleFactor;
    std::vector<std::unique_ptr<GrainGraphicsItem>> m_gfxGrains;
    std::unique_ptr<NozzleGraphicsItem> m_gfxNozzle;
	OpenBurnMotor* m_Motor;
    double m_MotorLen, m_MotorHeight; //in pixnels
    double m_currentSliceLocation;
    bool m_bAllowSlice;
};

