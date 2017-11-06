#pragma once
#include "src/propellant.h"
#include <QtMath>

enum GRAINTYPE
{
    GRAINTYPE_BATES = 0,
};
//represents a single propellant grain in a motor
class OpenBurnGrain
{
public:
    OpenBurnGrain(double length, double diameter, OpenBurnPropellant* prop, int inhibited = 0 );
    virtual ~OpenBurnGrain();

    virtual double GetLength();
    virtual double GetDiameter();
    virtual int GetInhibitedFaces();
    virtual OpenBurnPropellant* GetPropellantType();
    
    virtual void SetLength(double length);
    virtual void SetDiameter(double dia);
    virtual void SetInhibitedFaces(int faces);
    virtual void SetPropellantType(OpenBurnPropellant* prop);


    virtual double GetBurningSurfaceArea() = 0; //return the burning surface area of the propellant

    virtual double GetPortArea() = 0;
    virtual double GetVolume() = 0;
    virtual double GetHydraulicDiameter() = 0; //used for erosive burning calculation
    //burn faces and regress size params based on burn rate. run this once per time step
    //Returns true if burned successfully, false IFF if the grain burned out (used up all of it's propellant)
    virtual bool Burn(double timestep) = 0;

    virtual OpenBurnGrain* Clone() = 0;
    virtual void SetBurnRate(double steadyState, double erosiveFactor = 0);
//protected:
    double m_grainDia, m_grainLen;
    OpenBurnPropellant* m_prop;
    int m_numInhibitedFaces;

    double m_rNot, m_rErosive; //burn rates, additive (i.e r = r_0 + r_e)
};

//a cylindrical bates grain
class BatesGrain : public OpenBurnGrain
{
public:
    BatesGrain(double dia, double coredia, double len, OpenBurnPropellant* prop, int inhibitedfaces = 0);

    virtual ~BatesGrain() override;
    
    virtual double GetBurningSurfaceArea() override;
    virtual double GetPortArea() override;
    virtual double GetVolume() override;
    virtual double GetHydraulicDiameter() override;
    virtual bool Burn(double timestep) override;

    virtual BatesGrain* Clone() override;
    
    virtual double GetCoreDiameter();
    virtual void SetCoreDiameter(double dia);
private:
    double m_coreDia;
};

