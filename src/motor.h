#pragma once
#include <QJsonObject>
#include <QJsonArray>

#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>

#include "grain.h"
#include "nozzle.h"
enum KN_STATIC_CALC_TYPE
{
    KN_CALC_INITIAL,
    KN_CALC_MAX,
    KN_CALC_FINAL
};

typedef std::vector<OpenBurnGrain*> GrainVector;

//we inherit from QObject in order to use signals and slots
class OpenBurnMotor : public QObject
{
    Q_OBJECT
public:
    OpenBurnMotor();
    OpenBurnMotor(OpenBurnNozzle* nozz, GrainVector grains);
    ~OpenBurnMotor();

    double CalcStaticKn(KN_STATIC_CALC_TYPE type); //static kn calc based on regression derivitiave
    double CalcKn(); //instantaneous Kn calc for the current motor configuration

    double GetUpstreamBurningSurfaceArea(double xVal); //returns the burning surface area upstream of this point
    double GetBurningSurfaceArea();
    //if (copy), clones the grain objects and sets the grains as these new objects
    void SetGrains(GrainVector grains, bool copy = false);
    void SetNozzle(OpenBurnNozzle* nozz);

    void AddGrain(OpenBurnGrain* grain);
    void UpdateGrain(OpenBurnGrain* grain, int index);
    void SwapGrains(int idx1, int idx2);
    void RemoveGrain(OpenBurnGrain* grain);
    void RemoveGrain(int index);

    GrainVector GetGrains();
    OpenBurnGrain* GetGrainAtX(double x);
    OpenBurnNozzle* GetNozzle();
    const OpenBurnPropellant& GetAvgPropellant();

    bool HasNozzle() const;
    bool HasGrains() const;
    size_t GetNumGrains() const;

    double GetMotorLength() const;
    double GetMotorMajorDiameter() const;
    double GetMotorPropellantMass() const;
    double GetVolumeLoading() const;
    double GetPortThroatRatio() const;


    void ReadJSON(const QJsonObject& object, PropellantList* database);
    void WriteJSON(QJsonObject &object);

signals:
    void DesignReady();
    void DesignUpdated();
private:
    OpenBurnNozzle* m_Nozzle;
    GrainVector m_Grains;

    //WARNING - APPROXIMATION TIME --- average of all grains in motor because idk what im doing
    //weighted average based on mass of each segment of propellant
    void CalcAvgPropellant();
    OpenBurnPropellant m_AvgPropellant;

};
