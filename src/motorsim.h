#pragma once

#include <vector>

#include "grain.h"
#include "nozzle.h"

enum KN_STATIC_CALC_TYPE
{
    KN_CALC_INITIAL,
    KN_CALC_MAX,
    KN_CALC_FINAL
};
class MotorSim
{
public:
    MotorSim();

    static double CalcStaticKn(const std::vector<OpenBurnGrain*>& initial_grains, OpenBurnNozzle* nozzle, KN_STATIC_CALC_TYPE type);

    std::vector<OpenBurnGrain*> m_Grains; //all grains in the motor
    OpenBurnNozzle *m_Nozzle;
    
    bool HasGrains();
    bool HasNozzle();

    void SetGrains(std::vector<OpenBurnGrain*> grains);
    void SetNozzle(OpenBurnNozzle* nozz);
    
    void AddGrain(OpenBurnGrain* grain);
    void RemoveGrain(OpenBurnGrain *grain);
    void RemoveGrain(int index);
    void SwapGrains(int idx1, int idx2);

    //per unit area:
    double CalcMassFlux(double machNumber, double portArea);

    //per grain:
    //calculates linear burn rate for a given grain
    double CalcSteadyStateBurnRate(OpenBurnGrain* grain); //based on saint robert's law. No erosive burning
    double CalcErosiveBurnRateFactor(OpenBurnGrain* grain, double machNumber);

    //whole chamber:
    double CalcChamberPressure();
    double CalcKn(); //instantaneous kn calc, for grains in motor that have regressed a given ammount

    double GetMotorLength();
    double GetMotorMajorDiameter();
    double GetMotorPropellantMass();

    void RunSim(double timestep = 0.01f);

    size_t GetNumGrains() { return m_Grains.size(); }
private:
    //WARNING - APPROXIMATION TIME --- average of all grains in motor because idk what im doing

    //weighted average based on mass of propellant
    void CalcAvgPropellant();
    OpenBurnPropellant* m_avgPropellant;
};
