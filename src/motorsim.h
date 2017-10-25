#pragma once

#include <vector>

#include "grain.h"
#include "nozzle.h"

class MotorSim
{
public:
    MotorSim();
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
    double CalcErosiveBurnRateFactor(OpenBurnGrain* grain, double machNumber); //multiplicative law

    //whole chamber:
    double CalcChamberPressure();
    double CalcKn();

    void RunSim(double timestep = 0.01f);

    size_t GetNumGrains() { return m_Grains.size(); }
private:
    //WARNING - APPROXIMATION TIME --- average of all grains in motor because idk what im doing

    //weighted average based on mass of propellant
    OpenBurnPropellant* CalcAvgPropellant();
    OpenBurnPropellant* m_avgPropellant;
};
