#include "src/ui/graphics/graingraphicsitem.hpp"

GrainGraphicsItem::GrainGraphicsItem(int scale_factor, OpenBurnGrain* grain, bool crossSection, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_Grain(grain),
      m_Color(Qt::gray),
      m_ScaleFactor(scale_factor),
      m_isCrossSectionView(crossSection)
{
    setFlag(QGraphicsItem::ItemStacksBehindParent);
    setAcceptDrops(true);
}
void GrainGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_Grain)
    {
        if (m_isCrossSectionView)
        {
            m_Grain->PaintCrossSection(painter, m_ScaleFactor);
        }
        else
        {
            m_Grain->PaintFace(painter, m_ScaleFactor);
        }
    }
}
QRectF GrainGraphicsItem::boundingRect() const
{
    if (m_Grain)
    {
        if (m_isCrossSectionView)
        {
            return QRectF(0, 0, m_Grain->GetLength() * m_ScaleFactor, m_Grain->GetDiameter() * m_ScaleFactor);        
        }
        else
        {
            return QRectF(0, 0, m_Grain->GetDiameter() * m_ScaleFactor, m_Grain->GetDiameter() * m_ScaleFactor);
        }    
    }
    return QRectF();
}
void GrainGraphicsItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    update();
}
void GrainGraphicsItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    update();
}
void GrainGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);    
    update();
}
void GrainGraphicsItem::UpdateGrain(OpenBurnGrain *grain)
{
    m_Grain = grain;
    update();
}
