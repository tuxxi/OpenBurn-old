#include "units.h"

namespace OpenBurnUnits
{
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
//PRESSURE
    double ConvertPressure(PressureUnits_T oldUnits, PressureUnits_T newUnits, double value)
    {
        //TODO
    }
//TEMPERATURE
    double ConvertTemperature(TemperatureUnits_T oldUnits, TemperatureUnits_T newUnits, double value)
    {
        //TODO
    }
//FORCE
    double NewtonsToPounds(double value)
    {
        return POUNDS_PER_NEWTON * value;
    }
    double PoundsToNewtons(double value)
    {
        return NEWTONS_PER_POUND * value;
    }
//MASS
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
}

