#pragma once
#include <QString>
#include <QObject>
#include "src/motorsim.h"

//exports thrust/time curve to .eng
class EngExport : public QObject
{
    Q_OBJECT
public:
    EngExport(MotorSim* sim);
    void SetMotorDetails(double caseMass, const QString& mfgCode, const QString& mfgName);
    void WriteToEngFile(const QString& filename);
private:
    MotorSim* m_Simulator;
    double m_totalMass;
    QString m_mfgCode;
    QString m_mfgName;
};