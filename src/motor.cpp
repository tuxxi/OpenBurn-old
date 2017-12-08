#include <QDebug>
#include "motor.h"

OpenBurnMotor::OpenBurnMotor()
    : OpenBurnMotor(nullptr, GrainVector())
{

}
OpenBurnMotor::OpenBurnMotor(OpenBurnNozzle* nozz, GrainVector grains)
    : m_Nozzle(nozz),
      m_Grains(grains),
      m_AvgPropellant(nullptr)
{

}
OpenBurnMotor::~OpenBurnMotor()
{
    m_Grains.clear();
}
void OpenBurnMotor::SetGrains(const GrainVector& grains, bool copy)
{
    if (copy)
    {	//deep copy of grain pointers
		m_Grains.reserve(grains.size());
		for (const auto& grain : grains)
		{
			m_Grains.emplace_back(grain->Clone());
		}
    }
    else
    {
        m_Grains = grains;
        emit DesignUpdated();
    }
    CalcAvgPropellant();
	if (HasGrains() && HasNozzle())
	{
		emit DesignReady();
	}
}
void OpenBurnMotor::AddGrain(const std::shared_ptr<OpenBurnGrain>& grain)
{
    m_Grains.emplace_back(std::move(grain));
	emit GrainAdded(grain.get());
	UpdateDesign();
}
void OpenBurnMotor::AddGrain(const std::shared_ptr<OpenBurnGrain>& grain, int index)
{
	m_Grains.emplace(m_Grains.begin() + index, std::move(grain));
	emit GrainAdded(grain.get());
	UpdateDesign();
}

void OpenBurnMotor::UpdateGrain(const std::shared_ptr<OpenBurnGrain>& grain, int index)
{
	m_Grains.at(index) = grain;
	UpdateDesign();
}

void OpenBurnMotor::RemoveGrain(const std::shared_ptr<OpenBurnGrain>& grain)
{
	const int idx = GetGrainIndex(grain);
	if (idx != -1)
	{
		RemoveGrain(idx);
	}
}
void OpenBurnMotor::RemoveGrain(int index)
{
	qDebug() << "index is " << index;
	emit GrainRemoved(index);
    m_Grains.erase(m_Grains.begin() + index);
	UpdateDesign();
}

int OpenBurnMotor::GetGrainIndex(const std::shared_ptr<OpenBurnGrain>& grain)
{
	for (auto i = m_Grains.begin(); i != m_Grains.end(); ++i)
	{
		if (i->get() == grain.get())
		{
			return std::distance(m_Grains.begin(), i);
		}
	}
	return -1;
}

void OpenBurnMotor::SwapGrains(int oldPos, int newPos)
{
    std::swap(m_Grains[oldPos], m_Grains[newPos]);
    emit DesignUpdated();
}
void OpenBurnMotor::SetNozzle(OpenBurnNozzle* nozz)
{
    m_Nozzle = nozz;
	UpdateDesign();
}
OpenBurnGrain* OpenBurnMotor::GetGrainAtX(double x)
{
    double currentX = 0;
    for (auto& grain : m_Grains)
    {
        currentX += grain->GetLength();
        if (currentX >= x)
        {
            return grain.get();
        }
    }
    return nullptr;
}
double OpenBurnMotor::CalcStaticKn(KN_STATIC_CALC_TYPE type)
{
    double surfaceArea = 0;

    //http://www.nakka-rocketry.net/design1.html
    switch (type)
    {
    default:
    case KN_CALC_INITIAL:
        for (const auto& grain : m_Grains)
        {
            surfaceArea += grain->GetBurningSurfaceArea();
        }
        break;
    case KN_CALC_MAX:
        for (const auto& grain : m_Grains)
        {
            if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(grain.get()))
            {
                int inhibited = (2 - bates->GetInhibitedFaces());
                double webRegression = float(1.0f/6.0f) * (bates->GetLength() - inhibited * bates->GetCoreDiameter());
                double Ab_max_core_dia = bates->GetCoreDiameter() + (inhibited * webRegression);
                double Ab_max_len = bates->GetLength() - (inhibited * webRegression);
    
                double face_area = 0.25f * M_PI * ((bates->GetDiameter() * bates->GetDiameter()) - (Ab_max_core_dia * Ab_max_core_dia));
                double core_area = M_PI * Ab_max_core_dia * Ab_max_len;
    
                surfaceArea += core_area + inhibited * face_area;    
            }
            //other grain types
        }
        break;
    case KN_CALC_FINAL:
        for (const auto& grain : m_Grains)
        {
            if (CylindricalGrain* bates = dynamic_cast<CylindricalGrain*>(grain.get()))
            {
                double web_thickness = 0.5f * (bates->GetDiameter() - bates->GetCoreDiameter());
                double core_area = M_PI * bates->GetDiameter() * (bates->GetLength() - 2 * web_thickness);
                surfaceArea += core_area;    
            }            
        }
        break;
    }
    return surfaceArea / m_Nozzle->GetNozzleThroatArea();
}
double OpenBurnMotor::CalcKn()
{
    return GetBurningSurfaceArea() / m_Nozzle->GetNozzleThroatArea();
}
double OpenBurnMotor::GetBurningSurfaceArea()
{
    double surfaceArea = 0;
    for (const auto& grain : m_Grains)
    {
        if (!grain->GetIsBurnedOut())
        {
            surfaceArea += grain->GetBurningSurfaceArea();
        }
    }
    return surfaceArea;
}
//returns the mass flow upstream of this point
//x=0 at fwd end
double OpenBurnMotor::GetUpstreamMassFlow(double xVal)
{
    double massFlow = 0;
    double currentX = 0;
    for (const auto& grain : m_Grains)
    {
        double grainLen = grain->GetLength();
        if (currentX <= xVal) //we are still fwd of the sliced grain
        {
            double burningSurface = 0;
            if (std::ceil(currentX) < std::ceil(xVal - grainLen))
            {
                burningSurface = grain->GetBurningSurfaceArea();
            }
            else
            {
                burningSurface = grain->GetBurningSurfaceArea((currentX + grainLen) - xVal);
            }
            massFlow += burningSurface * grain->GetPropellantType().GetDensity() * grain->GetBurnRate();
        }
        currentX += grainLen;
    }
    return massFlow;
}
double OpenBurnMotor::GetTotalMassFlow()
{
    double massFlow = 0;
    for (const auto& grain : m_Grains)
    {
        massFlow += grain->GetBurningSurfaceArea() * 
            grain->GetPropellantType().GetDensity() * 
            grain->GetBurnRate();
    }
    return massFlow;
}

bool OpenBurnMotor::HasNozzle() const
{
    return m_Nozzle != nullptr;
}
bool OpenBurnMotor::HasGrains() const
{
    return m_Grains.size() != 0;
}
double OpenBurnMotor::GetMotorLength() const
{
    double len = 0;
    for (const auto& grain : m_Grains)
    {
        len += grain->GetLength();
    }
    return len;
}
double OpenBurnMotor::GetMotorMajorDiameter() const
{
    double dia = 0;
    for (const auto& grain : m_Grains)
    {
        //if the new grain's dia is larger than previously recorded dia, its the motor's major dia.
        dia < grain->GetDiameter() ? dia = grain->GetDiameter() : 0.0f;
    }
    return dia;
}
double OpenBurnMotor::GetMotorPropellantMass() const
{
    double mass = 0;
    for (const auto& grain : m_Grains)
    {
        mass += grain->GetVolume() * grain->GetPropellantType().GetDensity();
    }
    return mass;
}
size_t OpenBurnMotor::GetNumGrains() const
{
	return m_Grains.size();
}
GrainVector OpenBurnMotor::GetGrains() const
{
	return m_Grains;
}
OpenBurnNozzle* OpenBurnMotor::GetNozzle() const 
{
	return m_Nozzle;
}
const OpenBurnPropellant OpenBurnMotor::GetAvgPropellant() const
{
	return m_AvgPropellant;
}
double OpenBurnMotor::GetVolumeLoading() const
{
	double propellantVolume = 0;
	for (const auto& grain : m_Grains)
	{
		propellantVolume += grain->GetVolume();
	}
	double chamberVolume = .25f * M_PI * GetMotorMajorDiameter() * GetMotorMajorDiameter() * GetMotorLength();
	return propellantVolume / chamberVolume;
}
double OpenBurnMotor::GetPortThroatRatio() const
{
	//the nth grain in the list of n grains should always be the one at the nozzle end.
	return HasGrains() ? m_Grains[GetNumGrains() - 1]->GetPortArea() / m_Nozzle->GetNozzleThroatArea() : 0;
}
void OpenBurnMotor::UpdateDesign()
{
	emit DesignUpdated();
	CalcAvgPropellant();
	if (HasGrains() && HasNozzle())
	{
		emit DesignReady();
	}
	else
	{
		emit DesignNotReady();
	}
}
void OpenBurnMotor::CalcAvgPropellant()
{
    double w_a = 0, w_n = 0, w_cstar = 0, w_rho = 0, w_gmma = 0;
    //sum up all the propellant properties
    for (const auto& grain : m_Grains)
    {
        //"weight" each property based on the propellant mass in the motor
        double mass = grain->GetVolume() * grain->GetPropellantType().GetDensity();
        w_a += mass * grain->GetPropellantType().GetBurnRateCoef();
        w_n += mass * grain->GetPropellantType().GetBurnRateExp();
        w_cstar += mass * grain->GetPropellantType().GetCharVelocity();
        w_rho += mass * grain->GetPropellantType().GetDensity();
		w_gmma += mass * grain->GetPropellantType().GetSpecificHeatRatio();
    }
    double a = w_a / GetMotorPropellantMass();
    double n = w_n / GetMotorPropellantMass();
    double cstar = w_cstar / GetMotorPropellantMass();
    double rho = w_rho / GetMotorPropellantMass();
	double gmma = w_gmma / GetMotorPropellantMass();
    //double cpcv = w_cpcv / GetNumGrains();
    const QString debugName = "OPENBURNDEBUG::AVGPROP";
	m_AvgPropellant.SetBasicParams(a, n, cstar, rho, gmma);
	m_AvgPropellant.SetPropellantName(debugName);
}
void OpenBurnMotor::ReadJSON(const QJsonObject& object, PropellantList* database)
{
    PropellantList propellants;
    QJsonArray propellantArray = object["propellants"].toArray();
    for (auto i : propellantArray)
    {
        QJsonObject propellantObject = i.toObject();
        OpenBurnPropellant prop;
        prop.ReadJSON(propellantObject);
        for (auto k : *database)
        {
            if (prop.GetPropellantName() == k.GetPropellantName())
            {
                //duplicate stuff
            }
        }
        propellants.push_back(prop);
    }
    QJsonArray grainArray = object["grains"].toArray(); 
    for(auto grain : grainArray)
    {
        QJsonObject grainObject = grain.toObject();
        if (grainObject["_type"] == "BATES")
        {
            auto bates = std::make_shared<CylindricalGrain>();
            QString propellantName;
            bates->ReadJSON(grainObject, propellantName);
            for (auto prop : propellants)
            {
                if (propellantName == prop.GetPropellantName())
                {
                    bates->SetPropellantType(prop);
                }
            }
            AddGrain(bates);
        }
    }
    QJsonObject nozzleObject = object["nozzle"].toObject();
    if(nozzleObject["_type"] == "CONICAL")
    {
        ConicalNozzle* nozz = new ConicalNozzle(0, 0);
        nozz->ReadJSON(nozzleObject);
        SetNozzle(nozz);
    }    
}
void OpenBurnMotor::WriteJSON(QJsonObject &object)
{
    QJsonArray grainArray;
    PropellantList propellants;
    for(const auto& grain : m_Grains)
    {
        QJsonObject object;
        grain->WriteJSON(object);
        grainArray.append(object);

        if (propellants.empty()) //no propellants yet
        {
            propellants.push_back(grain->GetPropellantType());
        }
        else
        {
            for (auto prop : propellants)
            {
                if (!(prop == grain->GetPropellantType())) //we haven't seen this propellant before
                {
                    propellants.push_back(prop);
                }
            }
        }
    }
    QJsonObject nozzle;    
    if (HasNozzle())
    {
        m_Nozzle->WriteJSON(nozzle);        
    }

    QJsonArray propellantsArray;
    for(auto i : propellants)
    {
        QJsonObject prop;
        i.WriteJSON(prop);
        propellantsArray.append(prop);
    }
    object["grains"] = grainArray;    
    object["nozzle"] = nozzle;
    object["propellants"] = propellantsArray;    
}

