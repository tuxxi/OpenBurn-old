#pragma once
#include "propellant.h"
#include <QtMath>

//represents a single propellant grain in a motor
class OpenBurnGrain
{
public:
    ~OpenBurnGrain() {}

    virtual double GetLength() = 0;
    virtual double GetCoreDiameter() = 0;
    virtual double GetDiameter() = 0;
    virtual double GetSurfaceArea() = 0; //return the burning surface area of the propellant
    virtual double GetPortArea() = 0;
    virtual double GetVolume() = 0;
    virtual double GetHydraulicDiameter() = 0; //used for erosive burning calculation
    //burn faces and regress size params based on burn rate. run this once per time step
    //Returns true if burned successfully, false IFF if the grain burned out (used up all of it's propellant)
    virtual bool Burn(double timestep) = 0;

    virtual void SetBurnRate(double steadyState, double erosiveFactor = 0);
    OpenBurnPropellant GetPropellantType() { return m_prop; }
    OpenBurnPropellant SetPropellantType(OpenBurnPropellant prop) {m_prop = prop;}
protected:
    double m_rNot, m_rErosive; //burn rates, additive (i.e r = r0 + re)
    OpenBurnPropellant m_prop;
    OpenBurnGrain(OpenBurnPropellant prop)
        : m_prop(prop)
    {}

};

//a cylindrical bates grain
class BatesGrain : public OpenBurnGrain
{
public:
    BatesGrain(double dia, double coredia, double len, OpenBurnPropellant prop)
        : OpenBurnGrain(prop), m_grainDia(dia), m_coreDia(coredia), m_grainLen(len)
    {}
    ~BatesGrain() {}
    virtual double GetLength() override { return m_grainLen; }
    virtual double GetDiameter() override { return m_grainDia; }
    virtual double GetCoreDiameter() override { return m_coreDia; }
    virtual double GetSurfaceArea() override;
    virtual double GetPortArea() override;
    virtual double GetVolume() override;
    virtual double GetHydraulicDiameter() override;
    virtual bool Burn(double timestep) override;
private:
    double m_grainDia, m_coreDia, m_grainLen;
    int m_numInhibitedFaces;
};

