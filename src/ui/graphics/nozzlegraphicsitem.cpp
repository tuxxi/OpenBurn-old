#include "src/ui/graphics/nozzlegraphicsitem.h"

NozzleGraphicsItem::NozzleGraphicsItem(const OpenBurnNozzle* nozzle, int scale_factor, double nozzle_height,
	bool crossSection, QGraphicsItem* parent)
	: QGraphicsObject(parent),
      m_Color(Qt::lightGray),
      m_Nozzle(nozzle),
      m_NozzleHeight(nozzle_height),
      m_NozzleLength(0.f),
      m_isCrossSectionView(crossSection),
      m_scaleFactor(scale_factor)
{
    setFlag(QGraphicsItem::ItemStacksBehindParent);
}
void NozzleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_isCrossSectionView)
    {
        //draw entrance cone
        QBrush brush = QBrush(m_Color, Qt::DiagCrossPattern);
        QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        painter->setPen(pen);
        painter->setBrush(brush);
        double exitRadius = 0.5f * (m_Nozzle->GetNozzleExit() * m_scaleFactor);
        double throatRadius = 0.5f * (m_Nozzle->GetNozzleThroat() * m_scaleFactor);

        double throatHeightUpper = m_NozzleHeight * 0.5f + throatRadius;
        double throatHeightLower = m_NozzleHeight * 0.5f - throatRadius;

        double exitHeightUpper = m_NozzleHeight * 0.5f + exitRadius;
        double exitHeightLower = m_NozzleHeight * 0.5f - exitRadius;

        double convergentAngle = 25; // ?? allow user to change? maybe?
        double convergentLen = throatHeightUpper* qSin(qDegreesToRadians(convergentAngle));            
        
        //draw exit cone
	    const auto nozz = dynamic_cast<const ConicalNozzle*>(m_Nozzle);
        if (nozz)
        {        
            double divergentAngle = nozz->GetHalfAngle(); //nozz->GetHalfAngle();
            double throatLen = m_NozzleHeight * 0.2; //nozz->GetThroatLen();
        
            double divergentLen = exitRadius * (1.0f / qTan(qDegreesToRadians(divergentAngle))); //cot
            m_NozzleLength = convergentLen + throatLen + divergentLen;
            bool exitConeRestrained = exitRadius * 2.0f < m_NozzleHeight;
            QPoint nozzleLowerHalf[5] = 
            {   QPoint(0, 0), 
                QPoint(convergentLen, throatHeightLower), 
                QPoint(convergentLen + throatLen, throatHeightLower), 
                QPoint(m_NozzleLength, exitHeightLower),
                QPoint(m_NozzleLength, exitConeRestrained ? 0 : exitHeightLower) 
            };
            QPoint nozzleUpperHalf[5] = 
            {   QPoint(0, m_NozzleHeight), 
                QPoint(convergentLen, throatHeightUpper), 
                QPoint(convergentLen + throatLen, throatHeightUpper), 
                QPoint(m_NozzleLength, exitHeightUpper),
                QPoint(m_NozzleLength, exitConeRestrained ? m_NozzleHeight : exitHeightUpper),  
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
    //Because the viewport gets at the bounding rect of the nozzle before the nozzle has finished painting,
    //m_nozzleLen is not done being set in the paint() method.
    //We have to either force the viewport to repaint before boundingRect() is called,
    //or do this ugly pile of inefficent garbage
    //TODO: please fix me :( im a sad and lonely dynamic cast and i have no place being here
    double nozzleLen = 0;
    if (auto nozz = dynamic_cast<const ConicalNozzle*>(m_Nozzle))
    {
        double exitRadius = 0.5f * (m_Nozzle->GetNozzleExit() * m_scaleFactor);
        double throatRadius = 0.5f * (m_Nozzle->GetNozzleThroat() * m_scaleFactor);

        double convergentAngle = 25; // ?? allow user to change? maybe?
        double throatHeightUpper = m_NozzleHeight * 0.5f + throatRadius;
        double convergentLen = throatHeightUpper* qSin(qDegreesToRadians(convergentAngle));

        double divergentAngle = nozz->GetHalfAngle(); //nozz->GetHalfAngle();
        double throatLen = m_NozzleHeight * 0.2; //nozz->GetThroatLen();
        double divergentLen = exitRadius * (1.0f / qTan(qDegreesToRadians(divergentAngle))); //cot

        nozzleLen = convergentLen + throatLen + divergentLen;
    }
   return QRectF(0, 0, nozzleLen, m_NozzleHeight);
}
void NozzleGraphicsItem::UpdateNozzle(const OpenBurnNozzle* nozz)
{
    m_Nozzle = nozz;
	update();
}
void NozzleGraphicsItem::UpdateHeight(double height)
{
    m_NozzleHeight = height;
}
