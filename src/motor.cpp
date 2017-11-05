#include <QDebug>
#include "motor.h"

OpenBurnMotor::OpenBurnMotor()
    : m_Nozzle(nullptr), m_Grains(std::vector<OpenBurnGrain*>())
{

}
OpenBurnMotor::OpenBurnMotor(OpenBurnNozzle* nozz, std::vector<OpenBurnGrain*> grains)
    : m_Nozzle(nozz), m_Grains(grains)
{

}
OpenBurnMotor::~OpenBurnMotor()
{
    
}

void OpenBurnMotor::SetGrains(std::vector<OpenBurnGrain*> grains)
{
    m_Grains = grains;
}
void OpenBurnMotor::AddGrain(OpenBurnGrain* grain)
{
    m_Grains.push_back(grain);
}
void OpenBurnMotor::UpdateGrain(OpenBurnGrain* grain, int index)
{
    m_Grains[index] = grain;
}
void OpenBurnMotor::RemoveGrain(OpenBurnGrain *grain)
{
    for (auto i = m_Grains.begin(); i != m_Grains.end(); ++i)
    {
        if (*i == grain)
        {
            delete *i; 
            *i = nullptr;            
            m_Grains.erase(i);
        }
    }
}
void OpenBurnMotor::RemoveGrain(int index)
{
    delete m_Grains[index];
    m_Grains[index] = nullptr;    
    m_Grains.erase(m_Grains.begin() + index);
}
void OpenBurnMotor::SetNozzle(OpenBurnNozzle* nozz)
{
    m_Nozzle = nozz;
}

size_t OpenBurnMotor::GetNumGrains() { return m_Grains.size(); }
GrainVector OpenBurnMotor::GetGrains() { return m_Grains;}
OpenBurnNozzle* OpenBurnMotor::GetNozzle() { return m_Nozzle; }
const OpenBurnPropellant* OpenBurnMotor::GetAvgPropellant() { return m_avgPropellant; }

double OpenBurnMotor::CalcStaticKn(KN_STATIC_CALC_TYPE type)
{
    double surfaceArea = 0;

    //http://www.nakka-rocketry.net/design1.html
    switch (type)
    {
    default:
    case KN_CALC_INITIAL:
        for (auto i : m_Grains)
        {
            surfaceArea += i->GetBurningSurfaceArea();
        }
        break;
    case KN_CALC_MAX:
        for (auto i : m_Grains)
        {
            if (BatesGrain* bates = dynamic_cast<BatesGrain*>(i))
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
        for (auto i : m_Grains)
        {
            if (BatesGrain* bates = dynamic_cast<BatesGrain*>(i))
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
void OpenBurnMotor::SwapGrains(int oldPos, int newPos)
{
    //TODO: fix me
    qDebug() << "Grain idx " << oldPos << "swapping with idx " << newPos << "\n";
    qDebug() << "m_Grains has size: " << m_Grains.size() << "\n";

    std::swap(m_Grains[oldPos], m_Grains[newPos]); //std::swap uses move semantics afaik
}
bool OpenBurnMotor::HasNozzle() const
{
    return m_Nozzle != nullptr;
}
bool OpenBurnMotor::HasGrains() const
{
    return m_Grains.size() != 0;
}
double OpenBurnMotor::GetMotorLength()
{
    double len = 0;
    for (auto i : m_Grains)
    {
        len += i->GetLength();
    }
    return len;
}
double OpenBurnMotor::GetMotorMajorDiameter()
{
    double dia = 0;
    for (auto i : m_Grains)
    {
        //if the new grain's dia is larger than previously recorded dia, its the motor's major dia.
        dia < i->GetDiameter() ? dia = i->GetDiameter() : 0.0f;
    }
    return dia;
}
double OpenBurnMotor::GetMotorPropellantMass()
{
    double mass = 0;
    for (auto i : m_Grains)
    {
        mass += i->GetVolume() * i->GetPropellantType()->GetDensity();
    }
    return mass;
}
void OpenBurnMotor::CalcAvgPropellant()
{
    double weighted_a = 0, weighted_n = 0, weighted_cstar = 0, weighted_rho = 0;
    //sum up all the propellant properties
    for (auto  i : m_Grains)
    {
        //"weight" each property based on the propellant mass in the motor
        double mass = i->GetVolume() * i->GetPropellantType()->GetDensity();
        weighted_a += mass * i->GetPropellantType()->GetBurnRateCoef();
        weighted_n += mass * i->GetPropellantType()->GetBurnRateExp();
        weighted_cstar += mass * i->GetPropellantType()->GetCharVelocity();
        weighted_rho += mass * i->GetPropellantType()->GetDensity();
        //weighted_cpcv += mass * i->GetPropellantType()->GetSpecificHeatRatio();
    }
    double a = weighted_a / m_Grains.size();
    double n = weighted_n / m_Grains.size();
    double cstar = weighted_cstar / m_Grains.size();
    double rho = weighted_rho / m_Grains.size();
    //double cpcv = weighted_cpcv / m_Grains.size();
    
    delete m_avgPropellant; //clear old memory 
    m_avgPropellant = new OpenBurnPropellant(a, n, cstar, rho, "OPENBURNDEBUG::AVGPROP");
}
