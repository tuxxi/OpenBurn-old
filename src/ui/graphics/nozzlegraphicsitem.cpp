#include "src/ui/graphics/nozzlegraphicsitem.h"

NozzleGraphicsItem::NozzleGraphicsItem(OpenBurnNozzle* nozzle,
    int scale_factor, double nozzle_height, bool crossSection, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_color(Qt::lightGray), m_Nozzle(nozzle), m_nozzleHeight(nozzle_height), 
    m_isCrossSectionView(crossSection), m_scaleFactor(scale_factor)
{

}
void NozzleGraphicsItem::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

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

            double throatHeightUpper = m_nozzleHeight * 0.5f + throatRadius;
            double throatHeightLower = m_nozzleHeight * 0.5f - throatRadius;

            double exitHeightUpper = m_nozzleHeight * 0.5f + exitRadius;
            double exitHeightLower = m_nozzleHeight * 0.5f - exitRadius;

            double divergentAngle = nozz->GetHalfAngle(); //nozz->GetHalfAngle();
            double convergentAngle = 25; // ?? allow user to change? maybe?
            double throatLen = 20.0f; //nozz->GetThroatLen();
        
            double convergentLen = throatHeightUpper* qSin(qDegreesToRadians(convergentAngle));            
            double divergentLen = exitRadius * (1.0f / qTan(qDegreesToRadians(divergentAngle))); //cot
            m_nozzleLength = convergentLen + throatLen + divergentLen;
            bool exitConeRestrained = exitRadius * 2.0f < m_nozzleHeight;
            QPoint nozzleLowerHalf[5] = 
            {   QPoint(0, 0), 
                QPoint(convergentLen, throatHeightLower), 
                QPoint(convergentLen + throatLen, throatHeightLower), 
                QPoint(m_nozzleLength, exitHeightLower),
                QPoint(m_nozzleLength, exitConeRestrained ? 0 : exitHeightLower) 
            };
            QPoint nozzleUpperHalf[5] = 
            {   QPoint(0, m_nozzleHeight), 
                QPoint(convergentLen, throatHeightUpper), 
                QPoint(convergentLen + throatLen, throatHeightUpper), 
                QPoint(m_nozzleLength, exitHeightUpper),
                QPoint(m_nozzleLength, exitConeRestrained ? m_nozzleHeight : exitHeightUpper),  
            };
            painter->drawPolygon(nozzleUpperHalf, 5);
            painter->drawPolygon(nozzleLowerHalf, 5);
        }
    }
    else
    {
        //draw nozzle face!
    }

}
QRectF NozzleGraphicsItem::boundingRect() const
{
   return QRectF(0, 0, m_nozzleLength, m_nozzleHeight);
}
void NozzleGraphicsItem::UpdateNozzle(OpenBurnNozzle* nozz)
{
    m_Nozzle = nozz;
}
void NozzleGraphicsItem::UpdateHeight(double height)
{
    m_nozzleHeight = height;
}