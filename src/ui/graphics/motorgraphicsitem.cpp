#include "src/ui/graphics/motorgraphicsitem.h"

MotorGraphicsItem::MotorGraphicsItem(int scale_factor, QGraphicsItem *parent)
    : m_scaleFactor(scale_factor), m_MotorLen(0.0f), m_MotorHeight(0.0f), QGraphicsObject(parent),
      m_gfxGrains(NULL), m_gfxNozzle(nullptr)
{
    
}
QRectF MotorGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, m_MotorLen + (m_gfxNozzle ? m_gfxNozzle->boundingRect().width() : 0), m_MotorHeight);
}
void MotorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //draw the outline of the motor
    if (m_MotorLen > 0)
    {
        painter->setPen(QPen(Qt::black, 5, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));
        double len = m_MotorLen + (m_gfxNozzle ? m_gfxNozzle->boundingRect().width() : 0);
        painter->drawLine(0, 0, 0, m_MotorHeight); //forward end of motor
        painter->drawLine(0, 0, len, 0); //bottom line
        painter->drawLine(0, m_MotorHeight, len, m_MotorHeight); //top line    
    }
}
void MotorGraphicsItem::SetGrains(const std::vector<OpenBurnGrain*>& grains)
{  
    for (size_t i = !m_gfxGrains.empty() ? m_gfxGrains.size() : 0; i < grains.size(); ++i)
    {
        GrainGraphicsItem *newGrain = new GrainGraphicsItem(grains[i], m_scaleFactor, true, this);
        double len = newGrain->boundingRect().width();
        double height = newGrain->boundingRect().height();
        m_MotorHeight = qMax(height, m_MotorHeight); //if one grain is larger, that's our new motor diameter.

        newGrain->setPos(m_MotorLen, 0);
        m_gfxGrains.push_back(newGrain);
        m_MotorLen += len;
    }
    update();    
}
void MotorGraphicsItem::RemoveGrain(int index)
{
    int grainLen = m_gfxGrains[index]->boundingRect().width();  
    m_MotorLen -= grainLen;
    delete m_gfxGrains[index];
    m_gfxGrains.erase(m_gfxGrains.begin() + index);

    //shift grains after this one back in line
    for (int i = index; i < m_gfxGrains.size(); i++)
    {
        QPointF currentPos = m_gfxGrains[i]->pos();
        m_gfxGrains[i]->setPos(currentPos.rx() - grainLen, currentPos.ry());
    }
    update();    
}
void MotorGraphicsItem::SetScaleFactor(int scale)
{
    m_scaleFactor = scale;
}
void MotorGraphicsItem::SetNozzle(OpenBurnNozzle* nozzle)
{
    if (!m_gfxNozzle)
    {
        m_gfxNozzle = new NozzleGraphicsItem(nozzle, m_scaleFactor, m_MotorHeight, true, this);
    }
    m_gfxNozzle->UpdateNozzle(nozzle);    
    m_gfxNozzle->setPos(m_MotorLen, 0);
    update();
}
