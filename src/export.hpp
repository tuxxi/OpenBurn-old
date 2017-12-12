#pragma once
#include <QString>
#include <QObject>
#include "src/motorsim.hpp"

//exports thrust/time curve to .eng
class EngExport : public QObject
{
    Q_OBJECT
public:
    EngExport();
    EngExport(MotorSim* sim);
    EngExport(const EngExport& other);
    //kg, mm, mm
    void SetMotorDetails(double caseMass, int len, int dia, const QString& mfgCode, const QString& mfgName);
    void WriteToEngFile(const QString& filename) const;
private:
    MotorSim * m_Simulator;

    //eng file format uses mks for storage so these are in mm, newtons, kg ,etc
    double m_totalMass, m_propMass;
    int m_MotorLen, m_MotorDia;
    QString m_mfgCode;
    QString m_mfgName;
};