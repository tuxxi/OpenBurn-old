#pragma once
#include <QObject>
#include <QStringList>

namespace OpenBurnUnits
{
//length
    static const double MM_PER_INCH = 25.4;
    static const double INCHES_PER_MM = 1 / MM_PER_INCH;
    static const double INCHES_PER_FOOT = 12.0;
    static const double FEET_PER_INCH = 1.0 / INCHES_PER_FOOT;
    enum class LengthUnits_T
    {
        inches,
        millimeters,
        centimeters,
        feet,
        meters
    };
    static const QStringList g_kLengthUnits = (QStringList() <<
        QObject::tr("Inches (in)") <<
        QObject::tr("Millimeters (mm)") <<
        QObject::tr("Centimeters (cm)") <<
        QObject::tr("Feet (ft)") <<
        QObject::tr("Meters (m)"));

    double ConvertLength(LengthUnits_T prevUnits, LengthUnits_T newUnits, double value);
//angle
    enum class AngleUnits_T
    {
        degrees,
        radians
    };
    static const QStringList g_kAngleUnits = (QStringList() <<
        QObject::tr("Degrees") << 
        QObject::tr("Radians"));
//pressure
    enum class PressureUnits_T
    {
        psi,
        atmospheres,
        megapascals
    };
    static const QStringList g_kPressureUnits = (QStringList() <<
        QObject::tr("Pounds / sq-inch (psi)") <<
        QObject::tr("Atmospheres (atm)") <<
        QObject::tr("Megapascals (mPa)"));
    double ConvertPressure(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value);
//TEMPERATURE
    enum class TemperatureUnits_T
    {
        fahrenheit,
        celsius,
        kelvin
    };
    static const QStringList g_kTemperatureUnits = (QStringList() << 
        QObject::tr("Fahrenheit (F)") <<
        QObject::tr("Celsius (C)") <<
        QObject::tr("Kelvin (K)"));
    double ConvertTemperature(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value);
//FORCE
    static const double NEWTONS_PER_POUND = 4.44822;
    static const double POUNDS_PER_NEWTON = 1.0 / NEWTONS_PER_POUND;
    enum class ForceUnits_T
    {
        pounds_force,
        newtons
    };
    static const QStringList g_kForceUnits = (QStringList() << 
        QObject::tr("Pounds (lbs)") <<
        QObject::tr("Newtons (N)"));
    double NewtonsToPounds(double value);
    double PoundsToNewtons(double value);

//MASS
    static const double POUNDS_PER_KILOGRAM = 2.20462;
    static const double POUNDS_PER_SLUG = 32.174;
    static const double KILOGRAMS_PER_POUND = 1.0 / POUNDS_PER_KILOGRAM;
    static const double KILOGRAMS_PER_SLUG = 1.0 / POUNDS_PER_KILOGRAM * POUNDS_PER_SLUG;
    static const double SLUGS_PER_POUND = 1.0 / POUNDS_PER_SLUG;
    static const double SLUGS_PER_KILOGRAM = 1.0 / KILOGRAMS_PER_SLUG;
    enum class MassUnits_T
    {
        pounds_mass,
        kilograms,
        slugs
    };
    static const QStringList g_kMassUnits = (QStringList() << 
        QObject::tr("Pounds-mass (lbm)") <<
        QObject::tr("Kilograms (kg)") << 
        QObject::tr("Slugs (slug)"));
    double ConvertMass(MassUnits_T oldUnits, MassUnits_T newUnits, double value);
}