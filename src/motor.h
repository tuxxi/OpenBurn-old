#pragma once
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

    double CalcStaticKn(KN_STATIC_CALC_TYPE type);    

    double CalcKn(); //instantaneous Kn calc for the current motor configuration
    void SetGrains(GrainVector grains);
    void SetCopyGrains(GrainVector grains);
    void SetNozzle(OpenBurnNozzle* nozz);
    GrainVector GetGrains();
    OpenBurnNozzle* GetNozzle();

    bool HasGrains() const;
    bool HasNozzle() const;

    void AddGrain(OpenBurnGrain* grain);
    void UpdateGrain(OpenBurnGrain* grain, int index);
    void RemoveGrain(OpenBurnGrain* grain);
    void RemoveGrain(int index);
    void SwapGrains(int idx1, int idx2);
    size_t GetNumGrains();
    
    double GetMotorLength();
    double GetMotorMajorDiameter();
    double GetMotorPropellantMass();
    const OpenBurnPropellant* GetAvgPropellant();
signals:
    void SIG_DesignReady();
private:
    OpenBurnNozzle* m_Nozzle;
    GrainVector m_Grains;

    //WARNING - APPROXIMATION TIME --- average of all grains in motor because idk what im doing
    //weighted average based on mass of each segment of propellant
    void CalcAvgPropellant();
    OpenBurnPropellant* m_avgPropellant;

};