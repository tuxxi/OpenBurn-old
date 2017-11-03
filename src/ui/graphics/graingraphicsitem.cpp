#include "src/ui/graphics/graingraphicsitem.h"

GrainGraphicsItem::GrainGraphicsItem(OpenBurnGrain* grain, int scale_factor, bool crossSection, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_grain(grain), m_color(Qt::gray), 
    m_scaleFactor(scale_factor), m_isCrossSectionView(crossSection)
{
    setAcceptDrops(true);
}
void GrainGraphicsItem::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush brush = QBrush(m_color, Qt::FDiagPattern);
    QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(brush);

    double wid = m_grain->GetDiameter() * m_scaleFactor;
    double len = m_grain->GetLength() * m_scaleFactor;

    if (m_isCrossSectionView)
    {
        //draw grain rect
        painter->drawRect(0, 0, len, wid);
        if (BatesGrain* bates = dynamic_cast<BatesGrain*>(m_grain))
        {
            double corewid = bates->GetCoreDiameter() * m_scaleFactor;
            //"cut out" core using white brush
            painter->setBrush(Qt::white);
            painter->setPen(pen);    
            painter->drawRect(0, (wid/2) - (corewid/2), len, corewid);
        }
    }
    else
    {
        //face
    }
}
QRectF GrainGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, m_grain->GetLength() * m_scaleFactor, m_grain->GetDiameter() * m_scaleFactor);
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
