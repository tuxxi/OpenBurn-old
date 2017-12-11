#pragma once
#include <QObject>
#include <QStringList>
#include <QtMath>

namespace OpenBurnUnits
{
    //uses CRTP for compile-time static polymorphism
    template<typename T, class K>
    class OpenBurnUnit
    {
    public:
        OpenBurnUnit()
        {}
        OpenBurnUnit(const T& u)
            : unit(u)
        {}
        OpenBurnUnit(int u)
            : unit(T(u))
        {}
        static double Convert(T prevUnits, T newUnits, double value)
        {
            return K::Convert(prevUnits, newUnits, value);
        }
        static const QStringList GetAllUnitSymbols()
        {
            return K::GetAllUnitSymbols();
        }
        static const QStringList GetAllUnitNames()
        {
            return K::GetAllUnitNames();
        }
        static const QStringList GetUnits()
        {
            QStringList list = QStringList();
            for (int i = 0; i < GetAllUnitNames().size(); i++)
            {
                list << GetAllUnitNames()[i] + "(" + GetAllUnitSymbols()[i] + ")";
            }
            return list;
        }

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
            return GetAllUnitNames().at(int(unit));
        }
        operator int() const { return int(unit); }
        T unit;
    };
//=============================================================================
//*************LENGTH UNITS********************
//=============================================================================
    enum class LengthUnits_T
    {
        inches,
        millimeters,
        centimeters,
        feet,
        meters
    };
    class LengthUnits : public OpenBurnUnit<LengthUnits_T, LengthUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
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
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("in") <<
                QString("mm") <<
                QString("cm") <<
                QString("ft") <<
                QString("m");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Inches") <<
                QObject::tr("Millimeters") <<
                QObject::tr("Centimeters") <<
                QObject::tr("Feet") <<
                QObject::tr("Meters");
        }
    private:
        static constexpr double MM_PER_INCH = 25.4;
        static constexpr double INCHES_PER_MM = 1 / MM_PER_INCH;
        static constexpr double INCHES_PER_FOOT = 12.0;
        static constexpr double FEET_PER_INCH = 1.0 / INCHES_PER_FOOT;
    };

//=============================================================================
//*************ANGLE UNITS********************
//=============================================================================
    enum class AngleUnits_T
    {
        degrees,
        radians
    };
    class AngleUnits : public OpenBurnUnit<AngleUnits_T, AngleUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(AngleUnits_T oldUnits, AngleUnits_T newUnits, double value)
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
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("°") <<
                QString("rad");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Degrees") <<
                QObject::tr("Radians");
        }
    };
//=============================================================================
//*************PRESSURE UNITS********************
//=============================================================================
    enum class PressureUnits_T
    {
        psi,
        atmospheres,
        kilopascals,
        torr
    };
    class PressureUnits : public OpenBurnUnit<PressureUnits_T, PressureUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value)
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
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("psi") <<
                QString("atm") <<
                QString("kPa") <<
                QString("torr");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Pounds / inch²") <<
                QObject::tr("Atmospheres") <<
                QObject::tr("Kilopascals") <<
                QObject::tr("Torr");
        }
    private:
        static constexpr double KPA_PER_ATM = 101.325;
        static constexpr double KPA_PER_TORR = 133.3224;
        static constexpr double KPA_PER_PSI = 6.89476;
        static constexpr double ATM_PER_TORR = 760;
        static constexpr double TORR_PER_PSI = 51.7149;
        static constexpr double PSI_PER_ATM = 14.6959;

        static constexpr double ATM_PER_PSI = 1.0 / PSI_PER_ATM;
        static constexpr double ATM_PER_KPA = 1.0 / KPA_PER_ATM;
        static constexpr double PSI_PER_TORR = 1.0 / TORR_PER_PSI;
        static constexpr double PSI_PER_KPA = 1.0 / KPA_PER_PSI;
        static constexpr double TORR_PER_ATM = 1.0 / ATM_PER_TORR;
        static constexpr double TORR_PER_KPA = 1.0 / KPA_PER_TORR;
    };
//=============================================================================
//*************TEMPERATURE UNITS********************
//=============================================================================
    enum class TemperatureUnits_T
    {
        fahrenheit,
        celsius,
        kelvin
    };
    class TemperatureUnits : public OpenBurnUnit<TemperatureUnits_T, TemperatureUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value)
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
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("°F") <<
                QString("°C") <<
                QString("°K");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Fahrenheit") <<
                QObject::tr("Celsius") <<
                QObject::tr("Kelvin");
        }
    private:
        static constexpr double CELSIUS_KELVIN_OFFSET = 273.15;
        static constexpr double FAHRENHEIT_CELSIUS_OFFSET = 32;
        static constexpr double FAHRENHEIT_TO_CELSIUS = 5.0 / 9.0;
        static constexpr double CELSIUS_TO_FAHRENHEIT = 1.0 / FAHRENHEIT_TO_CELSIUS;
    };
//=============================================================================
//*************FORCE UNITS********************
//=============================================================================
    enum class ForceUnits_T
    {
        pounds_force,
        newtons
    };
    class ForceUnits : public OpenBurnUnit<ForceUnits_T, ForceUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(ForceUnits_T oldUnits, ForceUnits_T newUnits, double value)
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
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("lbs") <<
                QString("N");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Pounds") <<
                QObject::tr("Newtons");
        }
    private:
        static constexpr double NEWTONS_PER_POUND = 4.44822;
        static constexpr double POUNDS_PER_NEWTON = 1.0 / NEWTONS_PER_POUND;
    };
//=============================================================================
//*************MASS UNITS********************
//=============================================================================
    enum class MassUnits_T
    {
        pounds_mass,
        kilograms,
        slugs
    };
    class MassUnits : public OpenBurnUnit<MassUnits_T, MassUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(MassUnits_T oldUnits, MassUnits_T newUnits, double value)
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
                    case MassUnits_T::kilograms:
                        return KILOGRAMS_PER_POUND * value;
                    case MassUnits_T::slugs:
                        return SLUGS_PER_POUND * value;
                    }
                }
                case MassUnits_T::kilograms:
                {
                    switch(newUnits)
                    {
                    default:
                    case MassUnits_T::kilograms:
                        return value;
                    case MassUnits_T::pounds_mass:
                        return POUNDS_PER_KILOGRAM * value;
                    case MassUnits_T::slugs:
                        return SLUGS_PER_KILOGRAM * value;
                    }
                }

                case MassUnits_T::slugs:
                {
                    switch(newUnits)
                    {
                    default:
                    case MassUnits_T::slugs:
                        return value;
                    case MassUnits_T::kilograms:
                        return KILOGRAMS_PER_SLUG * value;
                    case MassUnits_T::pounds_mass:
                        return POUNDS_PER_SLUG * value;
                    }
                }
            }
        }
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("lbm") <<
                QString("kg") <<
                QString("slug");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Pounds-mass") <<
                QObject::tr("Kilograms") <<
                QObject::tr("Slugs");
        }
    private:
        static constexpr double POUNDS_PER_KILOGRAM = 2.20462;
        static constexpr double POUNDS_PER_SLUG = 32.174;
        static constexpr double KILOGRAMS_PER_POUND = 1.0 / POUNDS_PER_KILOGRAM;
        static constexpr double KILOGRAMS_PER_SLUG = 1.0 / POUNDS_PER_KILOGRAM * POUNDS_PER_SLUG;
        static constexpr double SLUGS_PER_POUND = 1.0 / POUNDS_PER_SLUG;
        static constexpr double SLUGS_PER_KILOGRAM = 1.0 / KILOGRAMS_PER_SLUG;
    };
//=============================================================================
//*************VELOCITY UNITS********************
//=============================================================================
    enum class VelocityUnits_T
    {
        inches_per_second,
        millimeters_per_second,
        feet_per_second,
        meters_per_second,
    };
    class VelocityUnits : public OpenBurnUnit<VelocityUnits_T, VelocityUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(VelocityUnits_T oldUnits, VelocityUnits_T newUnits, double value)
        {
            switch(oldUnits)
            {
                default:
                case VelocityUnits_T::inches_per_second:
                {
                    switch (newUnits)
                    {
                    default:
                    case VelocityUnits_T::inches_per_second:
                        return value;
                    case VelocityUnits_T::millimeters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::inches, LengthUnits_T::millimeters, value);
                    case VelocityUnits_T::feet_per_second:
                        return LengthUnits::Convert(LengthUnits_T::inches, LengthUnits_T::feet, value);
                    case VelocityUnits_T::meters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::inches, LengthUnits_T::meters, value);
                    }
                }
                case VelocityUnits_T::millimeters_per_second:
                {
                    switch (newUnits)
                    {
                    case VelocityUnits_T::inches_per_second:
                        return LengthUnits::Convert(LengthUnits_T::millimeters, LengthUnits_T::inches, value);
                    default:
                    case VelocityUnits_T::millimeters_per_second:
                        return value;
                    case VelocityUnits_T::feet_per_second:
                        return LengthUnits::Convert(LengthUnits_T::millimeters, LengthUnits_T::feet, value);
                    case VelocityUnits_T::meters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::meters, LengthUnits_T::meters, value);
                    }
                }
                case VelocityUnits_T::feet_per_second:
                {
                    switch (newUnits)
                    {
                    case VelocityUnits_T::inches_per_second:
                        return LengthUnits::Convert(LengthUnits_T::feet, LengthUnits_T::inches, value);
                    case VelocityUnits_T::millimeters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::feet, LengthUnits_T::millimeters, value);
                    default:
                    case VelocityUnits_T::feet_per_second:
                        return value;
                    case VelocityUnits_T::meters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::feet, LengthUnits_T::meters, value);
                    }
                }
                case VelocityUnits_T::meters_per_second:
                {
                    switch (newUnits)
                    {
                    case VelocityUnits_T::inches_per_second:
                        return LengthUnits::Convert(LengthUnits_T::meters, LengthUnits_T::inches, value);
                    case VelocityUnits_T::millimeters_per_second:
                        return LengthUnits::Convert(LengthUnits_T::meters, LengthUnits_T::millimeters, value);
                    case VelocityUnits_T::feet_per_second:
                        return LengthUnits::Convert(LengthUnits_T::meters, LengthUnits_T::feet, value);
                    default:
                    case VelocityUnits_T::meters_per_second:
                        return value;
                    }
                }
            }
        }

        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("in/sec") <<
                QString("mm/sec") <<
                QString("feet/sec") <<
                QString("m/sec");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Inches / Second") <<
                QObject::tr("Millimeters / Second") <<
                QObject::tr("Feet / Second") <<
                QObject::tr("Meters / Second");
        }
    };
    enum class MassFluxUnits_T
    {
        lbs_sec_sq_in,
        kg_sec_sq_meter
    };
//=============================================================================
//*************MASS FLUX UNITS********************
//=============================================================================
    class MassFluxUnits : public OpenBurnUnit<MassFluxUnits_T, MassFluxUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(MassFluxUnits_T oldUnits, MassFluxUnits_T newUnits, double value)
        {
            switch (oldUnits)
            {
            default:
            case MassFluxUnits_T::lbs_sec_sq_in:
            {
                switch (newUnits)
                {
                default:
                case MassFluxUnits_T::lbs_sec_sq_in:
                    return value;
                case MassFluxUnits_T::kg_sec_sq_meter:
                    return ENGLISH_PER_SI * value;
                }
            }
            case MassFluxUnits_T::kg_sec_sq_meter:
            {
                switch (newUnits)
                {
                case MassFluxUnits_T::lbs_sec_sq_in:
                    return SI_PER_ENGLISH * value;
                default:
                case MassFluxUnits_T::kg_sec_sq_meter:
                    return value;
                }
            }
            }
        }
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("lbs/sec/in²") <<
                QString("kg/sec/m²");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Pounds / Second / Square Inch") <<
                QObject::tr("Kilograms / Second / Square Meter");
        }
    private:
        static constexpr double SI_PER_ENGLISH = 0.00142233433;
        static constexpr double ENGLISH_PER_SI = 1.0 / SI_PER_ENGLISH;
    };
//=============================================================================
//*************DENSITY UNITS********************
//=============================================================================
    enum class DensityUnits_T
    {
        lbs_per_in_cu,
        kg_per_m_cu,
        g_per_cc,
        slugs_per_foot_cu
    };
    class DensityUnits : public OpenBurnUnit<DensityUnits_T, DensityUnits>
    {
    public:
        using OpenBurnUnit::OpenBurnUnit;
        static double Convert(DensityUnits_T oldUnits, DensityUnits_T newUnits, double value)
        {
            switch (oldUnits)
            {
            default:
            case DensityUnits_T::lbs_per_in_cu:
            {
                switch (newUnits)
                {
                default:
                case DensityUnits_T::lbs_per_in_cu:
                    return value;
                case DensityUnits_T::kg_per_m_cu:
                    return SI_PER_ENGLISH * value;
                case DensityUnits_T::g_per_cc:
                    return (SI_PER_ENGLISH / 1000) * value / 1000;
                case DensityUnits_T::slugs_per_foot_cu:
                    return LBSIN3_PER_SLUGFT3 * value;
                }
            }
            case DensityUnits_T::kg_per_m_cu:
            {
                switch (newUnits)
                {
                default:
                case DensityUnits_T::lbs_per_in_cu:
                    return ENGLISH_PER_SI * value;
                case DensityUnits_T::kg_per_m_cu:
                    return value;
                case DensityUnits_T::g_per_cc:
                    return value / 1000.0;
                case DensityUnits_T::slugs_per_foot_cu:
                    return ENGLISH_PER_SI * LBSIN3_PER_SLUGFT3 * value;
                }
            }
            case DensityUnits_T::g_per_cc:
            {
                switch (newUnits)
                {
                default:
                case DensityUnits_T::lbs_per_in_cu:
                    return 1000 * ENGLISH_PER_SI * value;
                case DensityUnits_T::kg_per_m_cu:
                    return 1000 * value;
                case DensityUnits_T::g_per_cc:
                    return value;
                case DensityUnits_T::slugs_per_foot_cu:
                    return (ENGLISH_PER_SI / 1000) * LBSIN3_PER_SLUGFT3 * value;
                }
            }
            case DensityUnits_T::slugs_per_foot_cu:
            {
                switch (newUnits)
                {
                default:
                case DensityUnits_T::lbs_per_in_cu:
                    return SLUGFT3_PER_LBSIN3 * value;
                case DensityUnits_T::kg_per_m_cu:
                    return SLUGFT3_PER_LBSIN3 * SI_PER_ENGLISH * value;
                case DensityUnits_T::g_per_cc:
                    return SLUGFT3_PER_LBSIN3 * (SI_PER_ENGLISH / 1000) * value;
                case DensityUnits_T::slugs_per_foot_cu:
                    return value;
                }
            }
            }
        }
        static const QStringList GetAllUnitSymbols()
        {
            return QStringList() <<
                QString("lbs/in³") <<
                QString("kg/m²") <<
                QString("g/cc") <<
                QString("slugs/ft³");
        }
        static const QStringList GetAllUnitNames()
        {
            return QStringList() <<
                QObject::tr("Pounds / Cubic Inch") <<
                QObject::tr("Kilograms / Cubic Meter") <<
                QObject::tr("Grams / Cubic Centimeter") <<
                QObject::tr("Slugs / Cubic Foot");
        }
    private:
        static constexpr double SI_PER_ENGLISH = 27679.9; //lbs/in^3 per kg/m^3
        static constexpr double ENGLISH_PER_SI = 1.0 / SI_PER_ENGLISH;
        static constexpr double LBSIN3_PER_SLUGFT3 = 53.7079;
        static constexpr double SLUGFT3_PER_LBSIN3 = 1.0 / LBSIN3_PER_SLUGFT3;
    };
}
