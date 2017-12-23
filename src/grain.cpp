#include <QDebug>

#include "grain.hpp"
OpenBurnGrain::OpenBurnGrain(double diameter, double length, OpenBurnPropellant prop, int inhibited)
    : m_GrainDia(diameter),
      m_GrainLen(length),
      m_Propellant(prop),
      m_NumInhibited(inhibited),
      m_isBurnedOut(false)
{

}
double OpenBurnGrain::GetLength() const { return m_GrainLen; }
double OpenBurnGrain::GetDiameter() const { return m_GrainDia; }
double OpenBurnGrain::GetHydraulicDiameter()
{
    return 4 *( GetPortArea() / (M_PI * m_GrainDia) );
}
int OpenBurnGrain::GetInhibitedFaces() const { return m_NumInhibited; }
OpenBurnPropellant& OpenBurnGrain::GetPropellantType() { return m_Propellant; }

bool OpenBurnGrain::GetIsBurnedOut() const { return m_isBurnedOut; }
double OpenBurnGrain::GetBurnRate() const { return m_rNot + m_rErosive; }

void OpenBurnGrain::SetLength(double length) { m_GrainLen = length; }
void OpenBurnGrain::SetDiameter(double dia) { m_GrainDia = dia; }
void OpenBurnGrain::SetInhibitedFaces(int faces) { m_NumInhibited = faces; }
void OpenBurnGrain::SetPropellantType(OpenBurnPropellant prop) { m_Propellant = prop; }
void OpenBurnGrain::SetBurnRate(double steadyState, double erosiveFactor)
{
    m_rNot = steadyState;
    m_rErosive = erosiveFactor;
}

CylindricalGrain::CylindricalGrain()
    : OpenBurnGrain(0, 0, OpenBurnPropellant()), m_CoreDia(0)
{}
// Bates Grains
CylindricalGrain::CylindricalGrain(double dia, double coredia, double len, OpenBurnPropellant prop, int inhibited)
    : OpenBurnGrain(dia, len, prop, inhibited), m_CoreDia(coredia)
{

}
double CylindricalGrain::GetCoreDiameter() { return m_CoreDia; }
void CylindricalGrain::SetCoreDiameter(double dia) { m_CoreDia = dia; }
double CylindricalGrain::GetBurningSurfaceArea()
{
    double face_area = 0.25f * M_PI * ((m_GrainDia * m_GrainDia) - (m_CoreDia * m_CoreDia));
    double core_area = M_PI * m_CoreDia * m_GrainLen;

    return core_area + ((2 - m_NumInhibited) * face_area);
}
double CylindricalGrain::GetBurningSurfaceArea(double xVal)
{
    if (xVal <= m_GrainLen) //only try to calculate if we're given an xval that is inside the grain volume
    {
        double face_area = 0.25f * M_PI * ((m_GrainDia * m_GrainDia) - (m_CoreDia * m_CoreDia));
        double core_area = M_PI * m_CoreDia * (m_GrainLen - xVal);
        return core_area + (m_NumInhibited > 0 ? 1 : 0) * face_area; //only one potential grain face will be upstream
    }
    else
    {
        return GetBurningSurfaceArea();
    }
}
double CylindricalGrain::GetPortArea()
{
    double coreRadius = m_CoreDia / 2.0f;
    return M_PI * coreRadius * coreRadius;
}
double CylindricalGrain::GetVolume()
{
    double radiusSq = (m_GrainDia / 2.f) * (m_GrainDia / 2.f);
    double coreRadiusSq = (m_CoreDia / 2.f) * (m_CoreDia / 2.f);
    return M_PI * m_GrainLen * (radiusSq - coreRadiusSq);
}
bool CylindricalGrain::Burn(double timestep)
{
    if (m_CoreDia <= m_GrainDia)
    {
        double burnDist = GetBurnRate() * timestep;
        
        m_CoreDia += (2 * burnDist);
        m_GrainLen -= burnDist * (2 - m_NumInhibited);
        return true;      
    }
    else
    {
        m_isBurnedOut = true;        
        return false;        
    }
}
bool CylindricalGrain::IsBurnedOut()
{
    return m_CoreDia <= m_GrainDia;
}
GrainPtr CylindricalGrain::Clone()
{
	return std::make_shared<CylindricalGrain>(*this);
}
std::unique_ptr<OpenBurnGrain> CylindricalGrain::CloneUnique()
{
    return std::make_unique<CylindricalGrain>(*this);
}
void CylindricalGrain::ReadJSON(const QJsonObject& object, QString& propellantNameReturn)
{
    m_CoreDia = object["corediameter"].toDouble();
    m_GrainDia = object["diameter"].toDouble();
    m_GrainLen = object["length"].toDouble();
    m_NumInhibited = object["inhibited"].toInt();
    propellantNameReturn = object["propellant"].toString();
}
void CylindricalGrain::WriteJSON(QJsonObject& object)
{
    object["_type"] = "BATES";
    object["length"] = m_GrainLen;
    object["diameter"] = m_GrainDia;
    object["corediameter"] = m_CoreDia;
    object["propellant"] = m_Propellant.GetPropellantName();
    object["inhibited"] = m_NumInhibited;
}
void CylindricalGrain::PaintFace(QPainter *painter, int scaleFactor)
{
    QBrush brush = QBrush(Qt::gray, Qt::FDiagPattern);
    QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(brush);

    double wid = GetDiameter() * scaleFactor;

    painter->drawEllipse(0, 0, wid, wid);
    const double corewid = GetCoreDiameter() * scaleFactor;
    const double centerPos = (wid - corewid) / 2.0;

    //"cut out" core using white brush
    painter->setBrush(Qt::white);
    painter->drawEllipse(centerPos, centerPos, corewid, corewid);
}
void CylindricalGrain::PaintCrossSection(QPainter *painter, int scaleFactor)
{
    QBrush brush = QBrush(Qt::gray, Qt::FDiagPattern);
    QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);

    double wid = GetDiameter() * scaleFactor;
    double len = GetLength() * scaleFactor;

    painter->drawRect(0, 0, len, wid);
    const double corewid = GetCoreDiameter() * scaleFactor;
    const double lower = (wid/2) - (corewid/2);
    const double upper = (wid/2) + (corewid/2);
    //"cut out" core using white brush
    painter->setBrush(Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, lower, len, corewid);

    painter->setPen(pen);
    //adding back the lines around the grain faces
    painter->drawLine(0, lower, len, lower);
    painter->drawLine(0, upper, len, upper);

    painter->drawLine(0, lower, 0, 0);
    painter->drawLine(len, lower, len, 0);
    painter->drawLine(0, upper, 0, wid);
    painter->drawLine(len, upper, len, wid);
}

//MoonBurner
MoonGrain::MoonGrain()
    : CylindricalGrain(0, 0, 0, OpenBurnPropellant()),
      m_CoreOffset(0)
{}
MoonGrain::MoonGrain(double dia, double coredia, double len, double offset, OpenBurnPropellant prop, int inhibited)
    : CylindricalGrain(dia, coredia, len, prop, inhibited),
      m_CoreOffset(offset)
{

}
double MoonGrain::GetCoreOffset() { return m_CoreOffset; }
void MoonGrain::SetCoreOffset(double offset) { m_CoreOffset = offset; }
double MoonGrain::GetBurningSurfaceArea()
{
    double face_area = 0.25f * M_PI * ((m_GrainDia * m_GrainDia) - (m_CoreDia * m_CoreDia));
    double core_area = M_PI * m_CoreDia * m_GrainLen;

    return core_area + ((2 - m_NumInhibited) * face_area);
}
double MoonGrain::GetBurningSurfaceArea(double xVal)
{
    if (xVal <= m_GrainLen) //only try to calculate if we're given an xval that is inside the grain volume
    {
        double face_area = 0.25f * M_PI * ((m_GrainDia * m_GrainDia) - (m_CoreDia * m_CoreDia));
        double core_area = M_PI * m_CoreDia * (m_GrainLen - xVal);
        return core_area + (m_NumInhibited > 0 ? 1 : 0) * face_area; //only one potential grain face will be upstream
    }
    else
    {
        return GetBurningSurfaceArea();
    }
}
double MoonGrain::GetPortArea()
{
    double coreRadius = m_CoreDia / 2.0f;
    return M_PI * coreRadius * coreRadius;
}
double MoonGrain::GetVolume()
{
    double radiusSq = (m_GrainDia / 2.f) * (m_GrainDia / 2.f);
    double coreRadiusSq = (m_CoreDia / 2.f) * (m_CoreDia / 2.f);
    return M_PI * m_GrainLen * (radiusSq - coreRadiusSq);
}
bool MoonGrain::Burn(double timestep)
{
    if (m_CoreDia <= m_GrainDia)
    {
        double burnDist = GetBurnRate() * timestep;

        m_CoreDia += (2 * burnDist);
        m_GrainLen -= burnDist * (2 - m_NumInhibited);
        return true;
    }
    else
    {
        m_isBurnedOut = true;
        return false;
    }
}
bool MoonGrain::IsBurnedOut()
{
    return m_CoreDia <= m_GrainDia;
}
GrainPtr MoonGrain::Clone()
{
    return std::make_shared<MoonGrain>(*this);
}
std::unique_ptr<OpenBurnGrain> MoonGrain::CloneUnique()
{
    return std::make_unique<MoonGrain>(*this);
}
void MoonGrain::ReadJSON(const QJsonObject& object, QString& propellantNameReturn)
{
    m_CoreDia = object["corediameter"].toDouble();
    m_CoreOffset = object["coreoffset"].toDouble();
    m_GrainDia = object["diameter"].toDouble();
    m_GrainLen = object["length"].toDouble();
    m_NumInhibited = object["inhibited"].toInt();

    propellantNameReturn = object["propellant"].toString();
}
void MoonGrain::WriteJSON(QJsonObject& object)
{
    object["_type"] = "MOON";
    object["length"] = m_GrainLen;
    object["diameter"] = m_GrainDia;
    object["corediameter"] = m_CoreDia;
    object["coreoffset"] = m_CoreOffset;
    object["propellant"] = m_Propellant.GetPropellantName();
    object["inhibited"] = m_NumInhibited;
}
void MoonGrain::PaintCrossSection(QPainter *painter, int scaleFactor)
{
    QBrush brush = QBrush(Qt::gray, Qt::FDiagPattern);
    QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);

    double wid = GetDiameter() * scaleFactor;
    double len = GetLength() * scaleFactor;

    painter->drawRect(0, 0, len, wid);
    const double offset = GetCoreOffset() * scaleFactor;
    const double corewid = GetCoreDiameter() * scaleFactor;
    const double lower = (wid/2) - (corewid/2) + offset;
    const double upper = (wid/2) + (corewid/2) + offset;
    //"cut out" core using white brush
    painter->setBrush(Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, lower, len, corewid);

    painter->setPen(pen);
    //adding back the lines around the grain faces
    painter->drawLine(0, lower, len, lower);
    painter->drawLine(0, upper, len, upper);

    painter->drawLine(0, lower, 0, 0);
    painter->drawLine(len, lower, len, 0);
    painter->drawLine(0, upper, 0, wid);
    painter->drawLine(len, upper, len, wid);
}
void MoonGrain::PaintFace(QPainter *painter, int scaleFactor)
{
    QBrush brush = QBrush(Qt::gray, Qt::FDiagPattern);
    QPen pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(brush);

    const double wid = GetDiameter() * scaleFactor;
    painter->drawEllipse(0, 0, wid, wid);

    const double offset = GetCoreOffset() * scaleFactor;
    const double corewid = GetCoreDiameter() * scaleFactor;
    const double centerPos = (wid - corewid) / 2.0;

    //"cut out" core using white brush
    painter->setBrush(Qt::white);
    painter->drawEllipse(centerPos, centerPos + offset, corewid, corewid);

}
