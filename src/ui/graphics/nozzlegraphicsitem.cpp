#include "src/ui/graphics/nozzlegraphicsitem.h"

NozzleGraphicsItem::NozzleGraphicsItem(OpenBurnNozzle* nozzle,
    int scale_factor, double nozzle_height, bool crossSection, QGraphicsItem *parent)
    : m_Nozzle(nozzle), m_scaleFactor(scale_factor),
      m_nozzleHeight(nozzle_height), m_color(Qt::lightGray), m_isCrossSectionView(crossSection),
      QGraphicsObject(parent)
{

}
void NozzleGraphicsItem::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_isCrossSectionView)
    {
        ConicalNozzle* nozz = dynamic_cast<ConicalNozzle*>(m_Nozzle);
        if (nozz)
        {
            //draw entrance cone
            QBrush brush = QBrush(m_color, Qt::DiagCrossPattern);
            QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
            painter->setPen(pen);
            painter->setBrush(brush);
            double exitRadius = 0.5f * (nozz->GetNozzleExit() * m_scaleFactor);
            double throatRadius = 0.5f * (nozz->GetNozzleThroat() * m_scaleFactor);
            double angle = 15; //nozz->GetHalfAngle();
            double throatAngle = 25;
            double len = exitRadius * qCos(qDegreesToRadians(angle));
            double throatLen = throatRadius * qSin(qDegreesToRadians(throatAngle));
            painter->drawRect(0, 0, throatLen + len, m_nozzleHeight);
            //fill in the white space around the nozzle
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(Qt::white));
            QPoint triangle_1[3] = { QPoint(0, 0), QPoint(throatLen, 0), QPoint(throatLen, throatRadius) };
            QPoint triangle_2[3] = { QPoint(0, m_nozzleHeight), QPoint(throatLen, m_nozzleHeight), QPoint(throatLen, m_nozzleHeight - throatRadius) };
            painter->drawPolygon(triangle_1, 3);
            painter->drawPolygon(triangle_2, 3);

        }
    }
    else
    {
        //draw nozzle face!
    }

}
QRectF NozzleGraphicsItem::boundingRect() const
{
   return QRectF(0, 0, 100, 100);
}
