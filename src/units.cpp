#include <QtMath>
#include "units.h"

namespace OpenBurnUnits
{
//=============================================================================
//*************LENGTH UNITS********************
//=============================================================================
    double ConvertLength(LengthUnits_T prevUnits, LengthUnits_T newUnits, double value)
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
                    return value / 10.0;
                    break;
                case LengthUnits_T::meters:
                    return value / 1000.0;
                    break;
                case LengthUnits_T::feet:
                    return MM_PER_INCH * FEET_PER_INCH * value;
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
                    return MM_PER_INCH * FEET_PER_INCH / 10.0 * value;
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
                    return MM_PER_INCH / 1000.0 * FEET_PER_INCH * value;
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
                    return MM_PER_INCH * INCHES_PER_FOOT * value;
                    break;
                case LengthUnits_T::centimeters:
                    return MM_PER_INCH / 10.0 * INCHES_PER_FOOT* value;
                    break;
                case LengthUnits_T::meters:
                    return MM_PER_INCH / 1000.0 * INCHES_PER_FOOT * value;
                    break;
                case LengthUnits_T::feet:
                    return value;
                    break;
                }
                break;
            }
        }
    }
    const QString GetLengthUnitName(LengthUnits_T unit)
    {
        return kLengthUnitNames.at(int(unit));
    }
    const QString GetLengthUnitSymbol(LengthUnits_T unit)
    {
        return kLengthUnitSymbols.at(int(unit));
    }
    const QStringList GetLengthUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kLengthUnitNames.size(); i++)
        {
            list << kLengthUnitNames[i] + "(" + kLengthUnitSymbols[i] + ")";
        }
        return list;
    }

//=============================================================================
//*************ANGLE UNITS********************
//=============================================================================
    double ConvertAngle(AngleUnits_T oldUnits, AngleUnits_T newUnits, double value)
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
    const QString GetAngleUnitName(AngleUnits_T units)
    {
        return kAngleUnitNames.at(int(units));
    }
    const QString GetAngleUnitSymbol(AngleUnits_T units)
    {
        return kAngleUnitSymbols.at(int(units));
    }
    const QStringList GetAngleUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kAngleUnitNames.size(); i++)
        {
            list << kAngleUnitNames[i] + "(" + kAngleUnitSymbols[i] + ")";
        }
        return list;
    }

//=============================================================================
//*************PRESSURE UNITS********************
//=============================================================================
    double ConvertPressure(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value)
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
    const QString GetPressureUnitName(PressureUnits_T units)
    {
        return kPressureUnitNames.at(int(units));
    }
    const QString GetPressureUnitSymbol(PressureUnits_T units)
    {
        return kPressureUnitSymbols.at(int(units));
    }
    const QStringList GetPressureUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kPressureUnitNames.size(); i++)
        {
            list << kPressureUnitNames[i] + "(" + kPressureUnitSymbols[i] + ")";
        }
        return list;
    }
//=============================================================================
//*************TEMPERATURE UNITS********************
//=============================================================================
    double ConvertTemperature(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value)
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
    const QString GetTemperatureUnitName(TemperatureUnits_T unit)
    {
        return kTemperatureUnitNames.at(int(unit));
    }
    const QString GetTemperatureUnitSymbol(TemperatureUnits_T unit)
    {
        return kTemperatureUnitSymbols.at(int(unit));
    }
    const QStringList GetTemperatureUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kTemperatureUnitNames.size(); i++)
        {
            list << kTemperatureUnitNames[i] + "(" + kTemperatureUnitSymbols[i] + ")";
        }
        return list;

    }

//=============================================================================
//*************FORCE UNITS********************
//=============================================================================
    double ConvertForce(ForceUnits_T oldUnits, ForceUnits_T newUnits, double value)
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
    const QString GetForceUnitName(ForceUnits_T unit)
    {
        return kForceUnitNames.at(int(unit));
    }
    const QString GetForceUnitSymbol(ForceUnits_T unit)
    {
        return kForceUnitSymbols.at(int(unit));
    }
    const QStringList GetForceUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kForceUnitNames.size(); i++)
        {
            list << kForceUnitNames[i] + "(" + kForceUnitSymbols[i] + ")";
        }
        return list;
    }
//=============================================================================
//*************MASS UNITS********************
//=============================================================================
    double ConvertMass(MassUnits_T oldUnits, MassUnits_T newUnits, double value)
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
    const QString GetMassUnitName(MassUnits_T unit)
    {
        return kMassUnitNames.at(int(unit));
    }
    const QString GetMassUnitSymbol(MassUnits_T unit)
    {
        return kMassUnitSymbols.at(int(unit));
    }
    const QStringList GetMassUnits()
    {
        QStringList list = QStringList();
        for (int i = 0; i < kMassUnitNames.size(); i++)
        {
            list << kMassUnitNames[i] + "(" + kMassUnitSymbols[i] + ")";
        }
        return list;
    }
}

