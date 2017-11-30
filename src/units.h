#pragma once
#include <QObject>
#include <QStringList>
#include <QtMath>

namespace OpenBurnUnits
{
    template<class T>
    class OpenBurnUnits
    {
    public:
        OpenBurnUnits()
        {}
        OpenBurnUnits(const T u)
            : unit(u)
        {}
        OpenBurnUnits(int u)
            : unit(T(u))
        {}

        static double Convert(T prevUnits, T newUnits, double value);
        static const QStringList GetAllUnitSymbols();
        static const QStringList GetAllUnitNames();

        double ConvertTo(T newUnits, double value)
        {
            return Convert(unit, newUnits, value);
        }
        double ConvertFrom(T oldUnits, double value)
        {
            return Convert(oldUnits, unit, value);
        }
        const QString GetUnitSymbol()
        {
            return GetAllUnitSymbols().at(int(unit));
        }
        const QString GetUnitName()
        {
            return GetAllNames().at(int(unit));
        }
        const QStringList GetUnits()
        {
            QStringList list = QStringList();
            for (int i = 0; i < GetAllUnitNames().size(); i++)
            {
                list << GetAllUnitNames()[i] + "(" + GetAllUnitSymbols()[i] + ")";
            }
            return list;
        }
        operator int() const { return int(unit); }
        T unit;
    };
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

    class LengthUnits : public OpenBurnUnits<LengthUnits_T>
    {
    public:
        LengthUnits() = default;
        LengthUnits(int u)
            : OpenBurnUnits<LengthUnits_T>(u)
        {}
        LengthUnits(const LengthUnits_T& u)
            : OpenBurnUnits<LengthUnits_T>(u)
        {}

        static double Convert(LengthUnits_T prevUnits, LengthUnits_T newUnits, double value)
        {
            switch(newUnits)
            {
                default:
                case LengthUnits_T::inches:
                {
                    switch(prevUnits)
                    {
                    default:
                    case LengthUnits_T::inches:
                        return value;
                        break;
                    case LengthUnits_T::millimeters:
                        return INCHES_PER_MM * value;
                        break;
                    case LengthUnits_T::centimeters:
                        return INCHES_PER_MM * 10.f * value;
                        break;
                    case LengthUnits_T::meters:
                        return INCHES_PER_MM * 1000.f * value;
                        break;
                    case LengthUnits_T::feet:
                        return INCHES_PER_FOOT * value;
                        break;
                    }
                    break;
                }
                case LengthUnits_T::millimeters:
                {
                    switch(prevUnits)
                    {
                    default:
                    case LengthUnits_T::inches:
                        return MM_PER_INCH * value;
                        break;
                    case LengthUnits_T::millimeters:
                        return value;
                        break;
                    case LengthUnits_T::centimeters:
                        return value * 10.0;
                        break;
                    case LengthUnits_T::meters:
                        return value * 1000.0;
                        break;
                    case LengthUnits_T::feet:
                        return MM_PER_INCH * INCHES_PER_FOOT * value;
                        break;
                    }
                    break;
                }
                case LengthUnits_T::centimeters:
                {
                    switch(prevUnits)
                    {
                    default:
                    case LengthUnits_T::inches:
                        return MM_PER_INCH / 10.0 * value;
                        break;
                    case LengthUnits_T::millimeters:
                        return value / 10.0;
                        break;
                    case LengthUnits_T::centimeters:
                        return value;
                        break;
                    case LengthUnits_T::meters:
                        return value / 100.0;
                        break;
                    case LengthUnits_T::feet:
                        return MM_PER_INCH * INCHES_PER_FOOT / 10.0 * value;
                        break;
                    }
                    break;
                }
                case LengthUnits_T::meters:
                {
                    switch(prevUnits)
                    {
                    default:
                    case LengthUnits_T::inches:
                        return MM_PER_INCH / 1000.0;
                        break;
                    case LengthUnits_T::millimeters:
                        return value / 1000.0;
                        break;
                    case LengthUnits_T::centimeters:
                        return value / 100.0;
                        break;
                    case LengthUnits_T::meters:
                        return value;
                        break;
                    case LengthUnits_T::feet:
                        return (MM_PER_INCH / 1000.0)* INCHES_PER_FOOT * value;
                        break;
                    }
                    break;
                }
                case LengthUnits_T::feet:
                {
                    switch(prevUnits)
                    {
                    default:
                    case LengthUnits_T::inches:
                        return FEET_PER_INCH * value;
                        break;
                    case LengthUnits_T::millimeters:
                        return INCHES_PER_MM * FEET_PER_INCH * value;
                        break;
                    case LengthUnits_T::centimeters:
                        return (INCHES_PER_MM * 10.0) * FEET_PER_INCH * value;
                        break;
                    case LengthUnits_T::meters:
                        return (INCHES_PER_MM * 1000.0) * FEET_PER_INCH * value;
                        break;
                    case LengthUnits_T::feet:
                        return value;
                        break;
                    }
                    break;
                }
            }
        }
        static const QStringList GetAllUnitSymbols() { return kLengthUnitSymbols; }
        static const QStringList GetAllUnitNames() { return kLengthUnitNames; }
    };

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

    class AngleUnits : public OpenBurnUnits<AngleUnits_T>
    {
    public:
        AngleUnits() {}
        AngleUnits(int u)
            : OpenBurnUnits<AngleUnits_T>(u) {}
        AngleUnits(const AngleUnits_T& u)
            : OpenBurnUnits<AngleUnits_T>(u) {}

        static double AngleUnits::Convert(AngleUnits_T oldUnits, AngleUnits_T newUnits, double value)
        {
            switch (oldUnits)
            {
                default:
                case AngleUnits_T::degrees:
                {
                    switch(newUnits)
                    {
                        default:
                        case AngleUnits_T::degrees:
                            return value;
                            break;
                        case AngleUnits_T::radians:
                            return qDegreesToRadians(value);
                            break;
                    }
                    break;
                }
                case AngleUnits_T::radians:
                {
                    switch (newUnits)
                    {
                        default:
                        case AngleUnits_T::radians:
                            return value;
                            break;
                        case AngleUnits_T::degrees:
                            return qRadiansToDegrees(value);
                            break;
                    }
                    break;
                }
            }
        }
        static const QStringList AngleUnits::GetAllUnitSymbols() { return kAngleUnitSymbols; }
        static const QStringList AngleUnits::GetAllUnitNames() { return kAngleUnitNames; }
    };
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

    class PressureUnits : public OpenBurnUnits<PressureUnits_T>
    {
    public:
        PressureUnits() {}
        PressureUnits(int u)
            : OpenBurnUnits<PressureUnits_T>(u) {}
        PressureUnits(const PressureUnits_T& u)
            : OpenBurnUnits<PressureUnits_T>(u) {}
        static double PressureUnits::Convert(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value)
        {
            switch(oldUnits)
            {
                default:
                case PressureUnits_T::psi:
                {
                    switch(newUnits)
                    {
                    default:
                    case PressureUnits_T::psi:
                        return value;
                        break;
                    case PressureUnits_T::atmospheres:
                        return ATM_PER_PSI * value;
                        break;
                    case PressureUnits_T::kilopascals:
                        return KPA_PER_PSI * value;
                        break;
                    case PressureUnits_T::torr:
                        return TORR_PER_PSI * value;
                        break;
                    }
                    break;
                }
                case PressureUnits_T::atmospheres:
                {
                    switch(newUnits)
                    {
                    default:
                    case PressureUnits_T::atmospheres:
                        return value;
                        break;
                    case PressureUnits_T::psi:
                        return PSI_PER_ATM * value;;
                        break;
                    case PressureUnits_T::kilopascals:
                        return KPA_PER_ATM * value;
                        break;
                    case PressureUnits_T::torr:
                        return TORR_PER_ATM * value;
                        break;
                    }
                    break;
                }
                case PressureUnits_T::kilopascals:
                {
                    switch(newUnits)
                    {
                    default:
                    case PressureUnits_T::kilopascals:
                        return value;
                        break;
                    case PressureUnits_T::psi:
                        return PSI_PER_KPA * value;;
                        break;
                    case PressureUnits_T::atmospheres:
                        return ATM_PER_KPA* value;
                        break;
                    case PressureUnits_T::torr:
                        return TORR_PER_KPA * value;
                        break;
                    }
                    break;
                }
                case PressureUnits_T::torr:
                {
                    switch(newUnits)
                    {
                    default:
                    case PressureUnits_T::torr:
                        return value;
                        break;
                    case PressureUnits_T::psi:
                        return PSI_PER_TORR * value;;
                        break;
                    case PressureUnits_T::atmospheres:
                        return ATM_PER_TORR* value;
                        break;
                    case PressureUnits_T::kilopascals:
                        return KPA_PER_TORR * value;
                        break;
                    }
                    break;
                }
            }
        }
        static const QStringList PressureUnits::GetAllUnitSymbols() { return kPressureUnitSymbols; }
        static const QStringList PressureUnits::GetAllUnitNames() { return kPressureUnitNames; }
    };
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

    class TemperatureUnits : public OpenBurnUnits<TemperatureUnits_T>
    {
    public:
        TemperatureUnits() {}
        TemperatureUnits(int u)
            : OpenBurnUnits<TemperatureUnits_T>(u) {}
        TemperatureUnits(const TemperatureUnits_T& u)
            : OpenBurnUnits<TemperatureUnits_T>(u) {}
        static double TemperatureUnits::Convert(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value)
        {
            switch(oldUnits)
            {
                default:
                case TemperatureUnits_T::fahrenheit:
                {
                    switch(newUnits)
                    {
                    default:
                    case TemperatureUnits_T::fahrenheit:
                        return value;
                        break;
                    case TemperatureUnits_T::celsius:
                        return FAHRENHEIT_TO_CELSIUS * (value - FAHRENHEIT_CELSIUS_OFFSET);
                        break;
                    case TemperatureUnits_T::kelvin:
                        return (FAHRENHEIT_TO_CELSIUS * (value - FAHRENHEIT_CELSIUS_OFFSET)) + CELSIUS_KELVIN_OFFSET;
                        break;
                    }
                    break;
                }
                case TemperatureUnits_T::celsius:
                {
                    switch(newUnits)
                    {
                    default:
                    case TemperatureUnits_T::celsius:
                        return value;
                        break;
                    case TemperatureUnits_T::fahrenheit:
                        return CELSIUS_TO_FAHRENHEIT * value + FAHRENHEIT_CELSIUS_OFFSET;
                        break;
                    case TemperatureUnits_T::kelvin:
                        return value + CELSIUS_KELVIN_OFFSET;
                        break;
                    }
                    break;
                }
                case TemperatureUnits_T::kelvin:
                {
                    switch(newUnits)
                    {
                    default:
                    case TemperatureUnits_T::kelvin:
                        return value;
                        break;
                    case TemperatureUnits_T::fahrenheit:
                        return (value - CELSIUS_KELVIN_OFFSET) * CELSIUS_TO_FAHRENHEIT + FAHRENHEIT_CELSIUS_OFFSET;
                        break;
                    case TemperatureUnits_T::celsius:
                        return value - CELSIUS_KELVIN_OFFSET;
                        break;
                    }
                    break;
                }
            }
        }
        static const QStringList TemperatureUnits::GetAllUnitSymbols() { return kTemperatureUnitSymbols; }
        static const QStringList TemperatureUnits::GetAllUnitNames() { return kTemperatureUnitNames; }


    };
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

    class ForceUnits : public OpenBurnUnits<ForceUnits_T>
    {
    public:
        ForceUnits() {}
        ForceUnits(int u)
            : OpenBurnUnits<ForceUnits_T>(u) {}
        ForceUnits(const ForceUnits_T& u)
            : OpenBurnUnits<ForceUnits_T>(u) {}
        static double ForceUnits::Convert(ForceUnits_T oldUnits, ForceUnits_T newUnits, double value)
        {
            switch (oldUnits)
            {
                default:
                case ForceUnits_T::newtons:
                {
                    switch(newUnits)
                    {
                        default:
                        case ForceUnits_T::newtons:
                            return value;
                            break;
                        case ForceUnits_T::pounds_force:
                            return POUNDS_PER_NEWTON * value;
                            break;
                    }
                    break;
                }
                case ForceUnits_T::pounds_force:
                {
                    switch (newUnits)
                    {
                        default:
                        case ForceUnits_T::pounds_force:
                            return value;
                            break;
                        case ForceUnits_T::newtons:
                            return NEWTONS_PER_POUND * value;
                            break;
                    }
                    break;
                }
            }
        }
        static const QStringList ForceUnits::GetAllUnitSymbols() { return kForceUnitSymbols; }
        static const QStringList ForceUnits::GetAllUnitNames() { return kForceUnitNames; }
    };
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

    class MassUnits : public OpenBurnUnits<MassUnits_T>
    {
    public:
        MassUnits() {}
        MassUnits(int u)
            : OpenBurnUnits<MassUnits_T>(u) {}
        MassUnits(const MassUnits_T& u)
            : OpenBurnUnits<MassUnits_T>(u) {}
        static double MassUnits::Convert(MassUnits_T oldUnits, MassUnits_T newUnits, double value)
        {
            switch(oldUnits)
            {
                default:
                case MassUnits_T::pounds_mass:
                {
                    switch(newUnits)
                    {
                    default:
                    case MassUnits_T::pounds_mass:
                        return value;
                        break;
                    case MassUnits_T::kilograms:
                        return KILOGRAMS_PER_POUND * value;
                        break;
                    case MassUnits_T::slugs:
                        return SLUGS_PER_POUND * value;
                        break;
                    }
                    break;
                }
                case MassUnits_T::kilograms:
                {
                    switch(newUnits)
                    {
                    default:
                    case MassUnits_T::kilograms:
                        return value;
                        break;
                    case MassUnits_T::pounds_mass:
                        return POUNDS_PER_KILOGRAM * value;
                        break;
                    case MassUnits_T::slugs:
                        return SLUGS_PER_KILOGRAM * value;
                        break;
                    }
                    break;
                }

                case MassUnits_T::slugs:
                {
                    switch(newUnits)
                    {
                    default:
                    case MassUnits_T::slugs:
                        return value;
                        break;
                    case MassUnits_T::kilograms:
                        return KILOGRAMS_PER_SLUG * value;
                        break;
                    case MassUnits_T::pounds_mass:
                        return POUNDS_PER_SLUG * value;
                        break;
                    }
                    break;
                }
            }
        }
        static const QStringList MassUnits::GetAllUnitSymbols() { return kMassUnitSymbols; }
        static const QStringList MassUnits::GetAllUnitNames() { return kMassUnitNames; }
    };
}
