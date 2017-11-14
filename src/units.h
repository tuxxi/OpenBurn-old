#include <QObject>
#include <QStringList>

namespace OpenBurnUnits
{
//length
    static const double MM_PER_INCH = 0.0393701;
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

    double LengthUnitsToInches(LengthUnits_T units, double value);
    double LengthUnitsToMillimeters(LengthUnits_T units, double value);
    double LengthUnitsToCentimeters(LengthUnits_T units, double value);
    double LengthUnitsToMeters(LengthUnits_T units, double value);
    double LengthUnitsToFeet(LengthUnits_T units, double value);
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
    double PressureUnitsToPsi(PressureUnits_T units, double value);
    double PressureUnitsToAtm(PressureUnits_T units, double value);
    double PressureUnitsToMpa(PressureUnits_T units, double value);
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
    double TemperatureUnitsToF(TemperatureUnits_T units, double value);
    double TemperatureUnitsToC(TemperatureUnits_T units, double value);
    double TemperatureUnitsToK(TemperatureUnits_T units, double value);

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
    double ForceUnitsToPounds(ForceUnits_T units, double value);
    double ForceUnitsToNewtons(ForceUnits_T units, double value);

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
    double MassUnitsToPoundsMass(MassUnits_T units, double value);
    double MassUnitsToKilograms(MassUnits_T units, double value);
    double MassUnitsToSlugs(MassUnits_T units, double value);
}