#pragma once
#include <QtMath>
#include <QJsonObject>

#include <memory>
#include <QtGui/QPainter>

#include "propellant.hpp"

class OpenBurnGrain;
typedef std::shared_ptr<OpenBurnGrain> GrainPtr;

//represents a single propellant grain in a motor
class OpenBurnGrain
{
public:
    OpenBurnGrain(double length, double diameter, OpenBurnPropellant prop, int inhibited = 0 );
    virtual ~OpenBurnGrain() = default;

    virtual double GetLength() const;
    virtual double GetDiameter() const;
    virtual int GetInhibitedFaces() const;
    virtual OpenBurnPropellant& GetPropellantType();
    virtual double GetBurnRate() const;

    virtual void SetLength(double length);
    virtual void SetDiameter(double dia);
    virtual void SetInhibitedFaces(int faces);
    virtual void SetPropellantType(OpenBurnPropellant prop);

    virtual bool GetIsBurnedOut() const;
    virtual double GetBurningSurfaceArea() = 0; //return the burning surface area of the propellant

    //burning surface area "upstream" of this x value
    //should return GetBurningSurfaceArea(void) if xVal is greater than grainLen
    virtual double GetBurningSurfaceArea(double xVal) = 0;

    virtual double GetPortArea() = 0;
    virtual double GetVolume() = 0;
    virtual double GetHydraulicDiameter(); //used for erosive burning calculation
    //burn faces and regress size params based on burn rate. run this once per time step
    //Returns true if burned successfully, false IFF if the grain burned out (used up all of it's propellant)
    virtual bool Burn(double timestep) = 0;
    virtual bool IsBurnedOut() = 0;
    virtual GrainPtr Clone() = 0;
    virtual std::unique_ptr<OpenBurnGrain> CloneUnique() = 0;
    virtual void SetBurnRate(double steadyState, double erosiveFactor = 0);

    virtual void ReadJSON(const QJsonObject& object, QString& propellantNameReturn) = 0;
    virtual void WriteJSON(QJsonObject &object) = 0;
    virtual void PaintCrossSection(QPainter* painter, int scaleFactor) = 0;
    virtual void PaintFace(QPainter* painter, int scaleFactor) = 0;

protected:
    double m_GrainDia, m_GrainLen;
    OpenBurnPropellant m_Propellant;
    int m_NumInhibited;

    double m_rNot, m_rErosive; //burn rates, additive (i.e r = r_0 + r_e)
    bool m_isBurnedOut;
};

//a cylindrical core "BATES" grain
class CylindricalGrain : public OpenBurnGrain
{
public:
    CylindricalGrain();
    CylindricalGrain(double dia, double coredia, double len, OpenBurnPropellant prop, int inhibited = 0);

    virtual ~CylindricalGrain() = default;
    
    virtual double GetBurningSurfaceArea() override;
    virtual double GetBurningSurfaceArea(double xVal) override;
    virtual double GetPortArea() override;
    virtual double GetVolume() override;
    virtual bool Burn(double timestep) override;
    virtual bool IsBurnedOut() override;
    virtual GrainPtr Clone() override;
    virtual std::unique_ptr<OpenBurnGrain> CloneUnique() override;

    virtual double GetCoreDiameter();
    virtual void SetCoreDiameter(double dia);

    virtual void ReadJSON(const QJsonObject& object, QString& propellantNameReturn) override;
    virtual void WriteJSON(QJsonObject &object) override;
    virtual void PaintCrossSection(QPainter* painter, int scaleFactor) override;
    virtual void PaintFace(QPainter* painter, int scaleFactor) override;
protected:
    double m_CoreDia;
};

//an offset cylindrical core "moonburner"
class MoonGrain : public CylindricalGrain
{
public:
    MoonGrain();
    MoonGrain(double dia, double coredia, double len, double offset, OpenBurnPropellant prop, int inhibited = 0);

    virtual ~MoonGrain() = default;

    virtual double GetBurningSurfaceArea() override;
    virtual double GetBurningSurfaceArea(double xVal) override;
    virtual double GetPortArea() override;
    virtual double GetVolume() override;
    virtual bool Burn(double timestep) override;
    virtual bool IsBurnedOut() override;

    virtual GrainPtr Clone() override;
    virtual std::unique_ptr<OpenBurnGrain> CloneUnique() override;

    virtual double GetCoreOffset();
    virtual void SetCoreOffset(double offset);

    virtual void ReadJSON(const QJsonObject& object, QString& propellantNameReturn) override;
    virtual void WriteJSON(QJsonObject &object) override;
    virtual void PaintCrossSection(QPainter* painter, int scaleFactor) override;
    virtual void PaintFace(QPainter* painter, int scaleFactor) override;

protected:
    double m_CoreOffset;
};

