#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>

#include <vector>
#include "src/ui/graphics/graingraphicsitem.h"
#include "src/grain.h"
#include "src/nozzle.h"

//this class represents the 2D cross sectional view of the whole motor
class MotorGraphicsItem : public QGraphicsObject
{
public:
    MotorGraphicsItem(int scale_factor, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    void SetGrains(const std::vector<OpenBurnGrain*> &grains);
    void RemoveGrain(int index);
    void SetNozzle(OpenBurnNozzle* nozzle);
    void SetScaleFactor(int scale);
    int GetScaleFactor() { return m_scaleFactor; }
    double m_MotorLen, m_MotorHeight; //in pixnels
    
private:
    int m_scaleFactor;
    
    std::vector<OpenBurnGrain*> m_Grains;
    std::vector<GrainGraphicsItem*> m_gfxGrains;
    
    OpenBurnNozzle *m_Nozzle;
};
