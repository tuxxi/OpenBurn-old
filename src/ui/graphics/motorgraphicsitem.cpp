#include "src/ui/graphics/motorgraphicsitem.h"


MotorGraphicsItem::MotorGraphicsItem(int scale_factor, bool allowSlice, QGraphicsItem *parent)
    : QGraphicsObject(parent),
    m_ScaleFactor(scale_factor),
    m_gfxNozzle(nullptr),
    m_MotorLen(0),
    m_MotorHeight(0),
    m_currentSliceLocation(0),
    m_bAllowSlice(allowSlice)
{

}
QRectF MotorGraphicsItem::boundingRect() const
{
    double nozzleLen = 0;
    if (m_gfxNozzle)
    {
        nozzleLen = m_gfxNozzle->boundingRect().width();
    }
    return QRectF(0, 0, m_MotorLen + nozzleLen, m_MotorHeight);
}
void MotorGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    SetXPos(event->pos().x());
}
void MotorGraphicsItem::SetXPos(double xPos)
{
    if (xPos >= m_MotorLen) { //we clicked off the nozzle end
        m_currentSliceLocation = std::ceil(m_MotorLen);
    }
    else if (xPos <= pos().x()) { //off the fwd end
        m_currentSliceLocation = 0;
    }
    else { //somewhere in the motor
        m_currentSliceLocation = xPos;
    }
    emit MotorXPosSliceUpdated(GetCurrentXPosSlice());
    update(boundingRect());

}
void MotorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //draw the outline of the motor
    if (m_MotorLen > 0)
    {
        painter->setPen(QPen(Qt::black, m_ScaleFactor / 20.0, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));
        double len = m_MotorLen + (m_gfxNozzle ? m_gfxNozzle->boundingRect().width() : 0);
        painter->drawLine(0, 0, 0, m_MotorHeight); //forward end of motor
        painter->drawLine(0, 0, len, 0); //bottom line
        painter->drawLine(0, m_MotorHeight, len, m_MotorHeight); //top line    
    }
    //draw current slice indicator
    if (m_currentSliceLocation > 0 && m_bAllowSlice)
    {
        painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(m_currentSliceLocation, -50, m_currentSliceLocation, m_MotorHeight + 50);
    }
}
void MotorGraphicsItem::SetGrains(const std::vector<OpenBurnGrain*>& grains)
{  
    //size_t i = !m_gfxGrains.empty() ? m_gfxGrains.size() : 0; i < grains.size(); ++i)
    for (size_t i = 0; i < grains.size(); i++)
    {
        GrainGraphicsItem* newGrain = new GrainGraphicsItem(grains[i], m_ScaleFactor, true, this);
        double len = newGrain->boundingRect().width();
        newGrain->setPos(m_MotorLen, 0);
        m_gfxGrains.push_back(newGrain);
        m_MotorLen += len;
    }
    CalculateMotorHeight();
    update(boundingRect());
}
void MotorGraphicsItem::RemoveGrain(int index)
{
    Q_UNUSED(index);
    /*
    int grainLen = m_gfxGrains[index]->boundingRect().width();  
    m_MotorLen -= grainLen;
    delete m_gfxGrains[index];
    m_gfxGrains[index] = nullptr;
    m_gfxGrains.erase(m_gfxGrains.begin() + index);

    //shift grains after this one back in line
    for (size_t i = index; i < m_gfxGrains.size(); i++)
    {
        QPointF currentPos = m_gfxGrains[i]->pos();
        m_gfxGrains[i]->setPos(currentPos.rx() - grainLen, currentPos.ry());
    }
    CalculateMotorHeight();
    */
}
void MotorGraphicsItem::RemoveAllGrains()
{
    for (size_t i = 0; i < m_gfxGrains.size(); i++)
    {
        delete m_gfxGrains[i];
        m_gfxGrains[i] = nullptr;
    }
    m_gfxGrains.clear();
    m_MotorLen = 0;
}
void MotorGraphicsItem::SetScaleFactor(int scale)
{
    m_ScaleFactor = scale;
}
void MotorGraphicsItem::SetNozzle(OpenBurnNozzle* nozzle)
{
    if (!m_gfxNozzle)
    {
        m_gfxNozzle = new NozzleGraphicsItem(nozzle, m_ScaleFactor, m_MotorHeight, true, this);
    }
    m_gfxNozzle->UpdateNozzle(nozzle);    
    m_gfxNozzle->setPos(m_MotorLen, 0);
    update(boundingRect());
}
//loops through all the grains in the motor and finds the _maximum_ diameter. this is the motor height.
void MotorGraphicsItem::CalculateMotorHeight()
{
    m_MotorHeight = 0.0f;
    for (auto* i : m_gfxGrains)
    {
        m_MotorHeight = qMax(i->boundingRect().height(), m_MotorHeight); //if one grain is larger, that's our new motor diameter.
    }
    if (m_gfxNozzle) m_gfxNozzle->UpdateHeight(m_MotorHeight);
}
void MotorGraphicsItem::UpdateGrains(const std::vector<OpenBurnGrain*>& grains)
{
    for (size_t i = 0; i < m_gfxGrains.size(); i++)
    {
        //we want the grain to stay in the middle of it's current location
        double pixelDiff = .5 * m_ScaleFactor * (m_gfxGrains[i]->GetGrain()->GetLength() - grains[i]->GetLength());
        m_gfxGrains[i]->setPos(m_gfxGrains[i]->pos().x() + pixelDiff, m_gfxGrains[i]->pos().y());
        m_gfxGrains[i]->UpdateGrain(grains[i]);
    }
}
double MotorGraphicsItem::GetCurrentXPosSlice()
{
    return m_currentSliceLocation / m_ScaleFactor;
}
