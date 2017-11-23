#pragma once
#include <QObject>
#include <QStringList>

namespace OpenBurnUnits
{
    //Generic functions, overload these for each unit
    template<class T>
    double Convert(T prevUnits, T newUnits, double value);
    template<class T>
    const QStringList GetAllUnitSymbols();
    template<class T>
    const QStringList GetAllUnitNames();
    
    template<class T>
    const QString GetUnitSymbol(T unit)
    {
        return GetAllUnitSymbols<T>().at(int(unit));
    }
    template<class T>
    const QString GetUnitName(T unit)
    {
        return GetAllUnitNames<T>().at(int(unit));
    }
    template<class T>
    const QStringList GetUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < GetAllUnitNames<T>().size(); i++)
        {
            list << GetAllUnitNames<T>()[i] + "(" + GetAllUnitSymbols<T>()[i] + ")";
        }
        return list;
    }
    
//=============================================================================
//*************LENGTH UNITS********************
//=============================================================================
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
    static const QStringList kLengthUnitSymbols = (QStringList() <<
        QString("in") <<
        QString("mm") <<
        QString("cm") <<
        QString("ft") <<
        QString("m"));
    static const QStringList kLengthUnitNames = (QStringList() <<
        QObject::tr("Inches") <<
        QObject::tr("Millimeters") <<
        QObject::tr("Centimeters") <<
        QObject::tr("Feet") <<
        QObject::tr("Meters"));

    template<>
    double Convert<LengthUnits_T>(LengthUnits_T prevUnits, LengthUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<LengthUnits_T>();
    template<>
    const QStringList GetAllUnitNames<LengthUnits_T>();

    double ConvertLength(LengthUnits_T prevUnits, LengthUnits_T newUnits, double value);
    const QString GetLengthUnitSymbol(LengthUnits_T unit);

//=============================================================================
//*************ANGLE UNITS********************
//=============================================================================
    enum class AngleUnits_T
    {
        degrees,
        radians
    };
    static const QStringList kAngleUnitSymbols = (QStringList() <<
        QString("°") << 
        QString("rad"));
    static const QStringList kAngleUnitNames = (QStringList() <<
        QObject::tr("Degrees") << 
        QObject::tr("Radians"));
    template<>
    double Convert<AngleUnits_T>(AngleUnits_T oldUnits, AngleUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<AngleUnits_T>();
    template<>
    const QStringList GetAllUnitNames<AngleUnits_T>();

    double ConvertAngle(AngleUnits_T oldUnits, AngleUnits_T newUnits, double value);
    const QString GetAngleUnitSymbol(AngleUnits_T unit);
//=============================================================================
//*************PRESSURE UNITS********************
//=============================================================================
    static const double KPA_PER_ATM = 101.325;
    static const double KPA_PER_TORR = 133.3224;
    static const double KPA_PER_PSI = 6.89476;
    static const double ATM_PER_TORR = 760;
    static const double TORR_PER_PSI = 51.7149;
    static const double PSI_PER_ATM = 14.6959; 

    static const double ATM_PER_PSI = 1.0 / PSI_PER_ATM;
    static const double ATM_PER_KPA = 1.0 / KPA_PER_ATM;
    static const double PSI_PER_TORR = 1.0 / TORR_PER_PSI;
    static const double PSI_PER_KPA = 1.0 / KPA_PER_PSI;
    static const double TORR_PER_ATM = 1.0 / ATM_PER_TORR; 
    static const double TORR_PER_KPA = 1.0 / KPA_PER_TORR;

    enum class PressureUnits_T
    {
        psi,
        atmospheres,
        kilopascals,
        torr
    };
    static const QStringList kPressureUnitSymbols = (QStringList() <<
        QString("psi") <<
        QString("atm") <<
        QString("kPa") <<
        QString("torr"));
    static const QStringList kPressureUnitNames = (QStringList() <<
        QObject::tr("Pounds / inch²") <<
        QObject::tr("Atmospheres") <<
        QObject::tr("Kilopascals") <<
        QObject::tr("Torr"));
    template<>
    double Convert<PressureUnits_T>(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<PressureUnits_T>();
    template<>
    const QStringList GetAllUnitNames<PressureUnits_T>();

    double ConvertPressure(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value);
    const QString GetPressureUnitSymbol(PressureUnits_T unit);
//=============================================================================
//*************TEMPERATURE UNITS********************
//=============================================================================
    static const double CELSIUS_KELVIN_OFFSET = 273.15;
    static const double FAHRENHEIT_CELSIUS_OFFSET = 32;
    static const double FAHRENHEIT_TO_CELSIUS = 5.0 / 9.0;
    static const double CELSIUS_TO_FAHRENHEIT = 1.0 / FAHRENHEIT_TO_CELSIUS;
    enum class TemperatureUnits_T
    {
        fahrenheit,
        celsius,
        kelvin
    };
    static const QStringList kTemperatureUnitSymbols = (QStringList() << 
        QString("°F") <<
        QString("°C") <<
        QString("°K"));
    static const QStringList kTemperatureUnitNames = (QStringList() << 
        QObject::tr("Fahrenheit") <<
        QObject::tr("Celsius") <<
        QObject::tr("Kelvin"));
    template<>
    double Convert<TemperatureUnits_T>(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<TemperatureUnits_T>();
    template<>
    const QStringList GetAllUnitNames<TemperatureUnits_T>();

    double ConvertTemperature(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value);
    const QString GetTemperatureUnitSymbol(TemperatureUnits_T unit);
//=============================================================================
//*************FORCE UNITS********************
//=============================================================================
    static const double NEWTONS_PER_POUND = 4.44822;
    static const double POUNDS_PER_NEWTON = 1.0 / NEWTONS_PER_POUND;
    enum class ForceUnits_T
    {
        pounds_force,
        newtons
    };
    static const QStringList kForceUnitSymbols = (QStringList() << 
        QString("lbs") <<
        QString("N"));
    static const QStringList kForceUnitNames = (QStringList() << 
        QObject::tr("Pounds") <<
        QObject::tr("Newtons"));
    template<>
    double Convert<ForceUnits_T>(ForceUnits_T oldUnits, ForceUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<ForceUnits_T>();
    template<>
    const QStringList GetAllUnitNames<ForceUnits_T>();

    double ConvertForce(ForceUnits_T oldUnits, ForceUnits_T newUnits, double value);
    const QString GetForceUnitSymbol(ForceUnits_T unit);
//=============================================================================
//*************MASS UNITS********************
//=============================================================================
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
    static const QStringList kMassUnitSymbols = (QStringList() << 
        QString("lbm") <<
        QString("kg") << 
        QString("slug"));
    static const QStringList kMassUnitNames = (QStringList() << 
        QObject::tr("Pounds-mass") <<
        QObject::tr("Kilograms") << 
        QObject::tr("Slugs"));
    template<>
    double Convert<MassUnits_T>(MassUnits_T oldUnits, MassUnits_T newUnits, double value);
    template<>
    const QStringList GetAllUnitSymbols<MassUnits_T>();
    template<>
    const QStringList GetAllUnitNames<MassUnits_T>();

    double ConvertMass(MassUnits_T oldUnits, MassUnits_T newUnits, double value);
    const QString GetMassUnitSymbol(MassUnits_T unit);
}