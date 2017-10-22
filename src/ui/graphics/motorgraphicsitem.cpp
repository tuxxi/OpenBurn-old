#include "src/ui/graphics/motorgraphicsitem.h"

MotorGraphicsItem::MotorGraphicsItem(int scale_factor, QGraphicsItem *parent)
    : m_scaleFactor(scale_factor), m_MotorLen(0.0f), m_MotorHeight(0.0f), QGraphicsObject(parent),
    m_Nozzle(nullptr), m_Grains(NULL)
{
    
}
QRectF MotorGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, m_MotorLen, m_MotorHeight);
}
void MotorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //draw the outline of the motor
    if (m_MotorLen > 0)
    {
        painter->setPen(QPen(Qt::black, 5, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(0, 0, 0, m_MotorHeight); //forward end of motor
        painter->drawLine(0, 0, m_MotorLen, 0); //bottom line
        painter->drawLine(0, m_MotorHeight, m_MotorLen, m_MotorHeight); //top line    
    }
    
    /*
    //draw nozzle:
    TODO: move to new class for nozzle
    if (m_Nozzle)
    {
        int newMotorLen = m_MotorLen + (qCos(qDegreesToRadians(70.0f)) * m_scaleFactor);
        int throatHeight = (m_Nozzle->GetNozzleThroat() / 2.0f) * m_scaleFactor;

        //draw entrance cone
        painter->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));        
        painter->drawLine(m_MotorLen, m_MotorHeight, 
            newMotorLen, m_MotorHeight - throatHeight);
        painter->drawLine(m_MotorLen, 0, 
            newMotorLen, throatHeight);
        
        //draw exit cone
        int exitHeight = (m_Nozzle->GetNozzleExit() / 2.0f)* m_scaleFactor;        
        int finalLen = newMotorLen + ((1.0f / qTan(qDegreesToRadians(15.0f) * exitHeight)));
        painter->drawLine(newMotorLen, m_MotorHeight - throatHeight, 
            finalLen, exitHeight);
        painter->drawLine(newMotorLen, throatHeight, 
            finalLen, m_MotorHeight - exitHeight);
    }
    */
}
void MotorGraphicsItem::SetGrains(const std::vector<OpenBurnGrain*> &grains)
{  
    m_Grains = grains;
    for (int i = m_gfxGrains.size(); i < m_Grains.size(); i++)
    {
        GrainGraphicsItem *newGrain = new GrainGraphicsItem(m_Grains[i], m_scaleFactor, true, this);
        double len = newGrain->boundingRect().width();
        double height = newGrain->boundingRect().height();
        m_MotorHeight = qMax(height, m_MotorHeight); //if one grain is larger, that's our new motor diameter.

        newGrain->setPos(m_MotorLen, 0);
        m_gfxGrains.push_back(newGrain);
        m_MotorLen += len;
    }
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
}
void MotorGraphicsItem::SetNozzle(OpenBurnNozzle* nozzle)
{
    m_Nozzle = nozzle;
}
void MotorGraphicsItem::SetScaleFactor(int scale)
{
    m_scaleFactor = scale;
}
