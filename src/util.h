#pragma once

#include <QStringList>
#include <QString>
#include <QObject>

namespace OpenBurnUtil
{
    static const QStringList g_kLengthUnits = (QStringList() <<
        QObject::tr("Inches (in)") <<
        QObject::tr("Millimeters (mm)") <<
        QObject::tr("Centimeters (cm)") <<
        QObject::tr("Feet (ft)") <<
        QObject::tr("Meters (m)"));

    static const QStringList g_kAngleUnits = (QStringList() <<
        QObject::tr("Degrees") << 
        QObject::tr("Radians"));

    static const QStringList g_kPressureUnits = (QStringList() <<
        QObject::tr("Pounds / sq inch (psi)") <<
        QObject::tr("Atmospheres (atm)") <<
        QObject::tr("Megapascals (mPa)"));

    static const QStringList g_kTemperatureUnits = (QStringList() << 
        QObject::tr("Fahrenheit (F)") <<
        QObject::tr("Celsius (C)") <<
        QObject::tr("Kelvin (K)"));

    double PoundsToSlugs(double lbs);
    double PoundsToNewtons(double lbs);
    //double InchesToMeters(double inches);

    char GetMotorClass(double nsec);
    static const double g_kGasConstantR = 8.3144621; //J / mol * k
    static const int g_kNumInterations = 200;
    static const int g_kSurfaceTemperature = 1000;//Average surface temp of burning propellant (degrees K)

}


