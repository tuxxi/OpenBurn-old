#pragma once
#include "propellant.h"
#include <QtMath>

//represents a single propellant grain in a motor
class OpenBurnGrain
{
public:
    ~OpenBurnGrain() {}

    virtual double GetSurfaceArea() = 0; //return the burning surface area of the propellant
    OpenBurnPropellant GetPropellantType() { return m_prop; }
    OpenBurnPropellant SetPropellantType(OpenBurnPropellant prop) {m_prop = prop;}
protected:
    OpenBurnPropellant m_prop;
    OpenBurnGrain(OpenBurnPropellant prop)
        : m_prop(prop)
    {}

};

//a cylindrical bates grain
class BatesGrain : OpenBurnGrain
{
public:
    BatesGrain(double dia, double coredia, double len, OpenBurnPropellant prop)
        : OpenBurnGrain(prop), m_grainDia(dia), m_coreDia(coredia), m_grainLen(len)
    {}
    ~BatesGrain() {}
    virtual double GetSurfaceArea() override;

private:
    double m_grainDia, m_coreDia, m_grainLen;
    int m_numInhibitedFaces;
};

