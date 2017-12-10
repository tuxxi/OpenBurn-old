#include "src/ui/graphics/motorgraphicsitem.h"


MotorGraphicsItem::MotorGraphicsItem(int scale_factor, OpenBurnMotor* motor, bool allowSlice, QGraphicsItem *parent)
    : QGraphicsObject(parent),
    m_ScaleFactor(scale_factor),
    m_gfxNozzle(nullptr),
	m_Motor(motor),
    m_MotorLen(0),
    m_MotorHeight(0),
    m_currentSliceLocation(0),
    m_bAllowSlice(allowSlice)
{
	connect(m_Motor, &OpenBurnMotor::GrainAdded,
		this, &MotorGraphicsItem::AddGrain);
	connect(m_Motor, &OpenBurnMotor::GrainRemoved,
		this, &MotorGraphicsItem::RemoveGrain);
	connect(m_Motor, &OpenBurnMotor::DesignUpdated,
		this, &MotorGraphicsItem::OnMotorUpdated);
	connect(m_Motor, &OpenBurnMotor::NozzleUpdated,
		this, &MotorGraphicsItem::OnNozzleUpdated);
	if (m_Motor->HasGrains())
	{
		SetGrains(m_Motor->GetGrains());
	}
	if (m_Motor->HasNozzle())
	{
		SetNozzle(m_Motor->GetNozzle());
	}
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
void MotorGraphicsItem::SetGrains(const GrainVector& grains)
{  
    for (auto& grain : grains)
    {
		AddGrain(grain.get());
    }
    CalculateMotorHeight();
}
void MotorGraphicsItem::AddGrain(OpenBurnGrain* grain)
{
	auto newGrain = std::make_unique<GrainGraphicsItem>(grain, m_ScaleFactor, true, this);
	newGrain->setPos(m_MotorLen, 0);
	m_gfxGrains.push_back(std::move(newGrain));
	m_MotorLen += m_gfxGrains.rbegin()->get()->boundingRect().width();
	CalculateMotorHeight();
}
void MotorGraphicsItem::RemoveGrain(int idx)
{
	m_MotorLen -= m_gfxGrains[idx].get()->boundingRect().width();
	m_gfxGrains.erase(m_gfxGrains.begin() + idx);
}
void MotorGraphicsItem::UpdateGrains(const GrainVector& grains)
{
	if (grains.size() > m_gfxGrains.size())
	{
		int diff = int(grains.size() - m_gfxGrains.size());
		for (auto i = grains.rbegin(); i != grains.rbegin() + diff; ++i) //reverse iterator operator+() goes backwwards :<
		{
			AddGrain(i->get());
		}
	}
	m_MotorLen = 0;
	for (size_t i = 0; i < grains.size(); i++)
	{
		m_gfxGrains[i]->UpdateGrain(grains[i].get());
		m_gfxGrains[i]->setPos(m_MotorLen, 0);
		m_MotorLen += m_gfxGrains[i]->boundingRect().width();
	}
	CalculateMotorHeight();
}
void MotorGraphicsItem::RemoveAllGrains()
{
    m_gfxGrains.clear();
    m_MotorLen = 0;
	HideNozzle();
}
void MotorGraphicsItem::SetScaleFactor(int scale)
{
    m_ScaleFactor = scale;
}
void MotorGraphicsItem::SetNozzle(const OpenBurnNozzle* nozzle)
{
    if (!m_gfxNozzle)
    {
        m_gfxNozzle = std::make_unique<NozzleGraphicsItem>(nozzle, m_ScaleFactor, m_MotorHeight, true, this);
    }
    m_gfxNozzle->UpdateNozzle(nozzle);    
    m_gfxNozzle->setPos(m_MotorLen, 0);
    update(boundingRect());
}
void MotorGraphicsItem::HideNozzle()
{
	m_gfxNozzle.reset();
}

//loops through all the grains in the motor and finds the _maximum_ diameter. this is the motor height.
void MotorGraphicsItem::CalculateMotorHeight()
{
    m_MotorHeight = 0.0f;
    for (auto& i : m_gfxGrains)
    {
        m_MotorHeight = qMax(i->boundingRect().height(), m_MotorHeight); //if one grain is larger, that's our new motor diameter.
    }
    if (m_gfxNozzle) m_gfxNozzle->UpdateHeight(m_MotorHeight);
	update(boundingRect());
}
void MotorGraphicsItem::BurnGrains(const GrainVector& grains)
{
	if (grains.size() != m_gfxGrains.size()) return;

    for (size_t i = 0; i < grains.size(); i++)
    {
        //we want the grain to stay in the middle of it's current location
        double pixelDiff = .5 * m_ScaleFactor * (m_gfxGrains[i]->GetGrain()->GetLength() - grains[i]->GetLength());
        m_gfxGrains[i]->setPos(m_gfxGrains[i]->pos().x() + pixelDiff, m_gfxGrains[i]->pos().y());
        m_gfxGrains[i]->UpdateGrain(grains[i].get());
    }
}
double MotorGraphicsItem::GetCurrentXPosSlice()
{
    return m_currentSliceLocation / m_ScaleFactor;
}
void MotorGraphicsItem::OnMotorUpdated()
{
	if (!m_Motor->HasGrains())
	{
		RemoveAllGrains();
		HideNozzle();
	}
	else if (m_Motor->HasNozzle())
	{
		SetNozzle(m_Motor->GetNozzle());
	}
	UpdateGrains(m_Motor->GetGrains());
}

void MotorGraphicsItem::OnNozzleUpdated(OpenBurnNozzle* nozzle)
{
	if (nozzle == nullptr)
	{
		HideNozzle();
	}
	else
	{
		SetNozzle(nozzle);
	}
}
