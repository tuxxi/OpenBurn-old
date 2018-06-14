#pragma once
#include <QString>
#include <QObject>
#include "motorsim.hpp"
#include "application.hpp"

//exports thrust/time curve to .eng
class EngExport : public QObject
{
    Q_OBJECT
public:
    EngExport() = delete;
    EngExport(OpenBurnApplication& app);
    EngExport(const EngExport& other);
    //kg, mm, mm
    void SetMotorDetails(double caseMass, int len, int dia, const QString& mfgCode, const QString& mfgName);
    void WriteToEngFile(const QString& filename) const;
private:
    OpenBurnApplication& m_app;

    //eng file format uses mks for storage so these are in mm, newtons, kg ,etc
    double m_totalMass, m_propMass;
    int m_MotorLen, m_MotorDia;
    QString m_mfgCode;
    QString m_mfgName;
};