#include "src/ui/graphics/graingraphicsitem.h"

GrainGraphicsItem::GrainGraphicsItem(OpenBurnGrain* grain, int scale_factor, bool crossSection, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_Grain(grain),
      m_Color(Qt::gray),
      m_ScaleFactor(scale_factor),
      m_isCrossSectionView(crossSection)
{
    setAcceptDrops(true);
}
void GrainGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_Grain)
    {
        QBrush brush = QBrush(m_Color, Qt::FDiagPattern);
        QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        painter->setPen(Qt::NoPen);
        painter->setBrush(brush);
    
        double wid = m_Grain->GetDiameter() * m_ScaleFactor;
        double len = m_Grain->GetLength() * m_ScaleFactor;
    
        if (m_isCrossSectionView)
        {
            //draw grain rect
            painter->drawRect(0, 0, len, wid);
            if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(m_Grain))
            {
                double corewid = bates->GetCoreDiameter() * m_ScaleFactor;
                //"cut out" core using white brush
                painter->setBrush(Qt::white);
                painter->setPen(Qt::NoPen);
                painter->drawRect(0, (wid/2) - (corewid/2), len, corewid);

                painter->setPen(pen);
                //adding back the lines around the grain faces
                painter->drawLine(0, (wid/2) - (corewid/2), len, (wid/2) - (corewid/2));
                painter->drawLine(0, (wid/2) + (corewid/2), len, (wid/2) + (corewid/2));

                painter->drawLine(0, (wid/2) - (corewid/2), 0, 0);
                painter->drawLine(len, (wid/2) - (corewid/2), len, 0);
                painter->drawLine(0, (wid/2) + (corewid/2), 0, wid);
                painter->drawLine(len, (wid/2) + (corewid/2), len, wid);
            }
            else
            {
            //other grain types
            }
        }
        else
        {
            painter->setPen(pen);
            painter->drawEllipse(0, 0, wid, wid);
            if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(m_Grain))
            {
                double corewid = bates->GetCoreDiameter() * m_ScaleFactor;
                //"cut out" core using white brush
                painter->setBrush(Qt::white);
                painter->drawEllipse((wid - corewid) /2.0f, (wid - corewid) / 2.0f, corewid, corewid);
            }
            else
            {
            //other grain types
            }
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
