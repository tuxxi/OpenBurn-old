#include "units.h"

namespace OpenBurnUnits
{
//LENGTH
    double LengthUnitsToInches(LengthUnits_T units, double value)
    {
        switch(units)
        {
        default:
        case LengthUnits_T::inches:
            return value;
            break;
        case LengthUnits_T::millimeters:
            return MM_PER_INCH * value;
            break;
        case LengthUnits_T::centimeters:
            return MM_PER_INCH * 10.f * value;
            break;
        case LengthUnits_T::meters:
            return MM_PER_INCH * 1000.f * value;
            break;
        case LengthUnits_T::feet:
            return FEET_PER_INCH * value;
            break;
        }
    }
    double LengthUnitsToMillimeters(LengthUnits_T units, double value);
    double LengthUnitsToCentimeters(LengthUnits_T units, double value);
    double LengthUnitsToMeters(LengthUnits_T units, double value);
    double LengthUnitsToFeet(LengthUnits_T units, double value);
//PRESSURE
    double PressureUnitsToPsi(PressureUnits_T units, double value);
    double PressureUnitsToAtm(PressureUnits_T units, double value);
    double PressureUnitsToMpa(PressureUnits_T units, double value);
//TEMPERATURE
    double TemperatureUnitsToF(TemperatureUnits_T units, double value);
    double TemperatureUnitsToC(TemperatureUnits_T units, double value);
    double TemperatureUnitsToK(TemperatureUnits_T units, double value);
//FORCE
    double ForceUnitsToPounds(ForceUnits_T units, double value)
    {
        switch(units)
        {
        default:
        case ForceUnits_T::pounds_force:
            return value;
            break;
        case ForceUnits_T::newtons:
            return POUNDS_PER_NEWTON * value;
            break;
        }
    }
    double ForceUnitsToNewtons(ForceUnits_T units, double value)
    {
        switch (units)
        {
        default:
        case ForceUnits_T::newtons:
            return value;
            break;
        case ForceUnits_T::pounds_force:
            return NEWTONS_PER_POUND * value;
            break;
        }
    }
//MASS
    double MassUnitsToPoundsMass(MassUnits_T units, double value)
    {
        switch(units)
        {
        default:
        case MassUnits_T::pounds_mass:
            return value;
            break;
        case MassUnits_T::kilograms:
            return POUNDS_PER_KILOGRAM * value;
            break;
        case MassUnits_T::slugs:
            return POUNDS_PER_SLUG * value;
            break;
        }
    }
    double MassUnitsToKilograms(MassUnits_T units, double value)
    {
        switch(units)
        {
        default:
        case MassUnits_T::kilograms:
            return value;
            break;
        case MassUnits_T::pounds_mass:
            return KILOGRAMS_PER_POUND * value;
            break;
        case MassUnits_T::slugs:
            return KILOGRAMS_PER_SLUG * value;
            break;
        }
    }
    double MassUnitsToSlugs(MassUnits_T units, double value)
    {
        switch(units)
        {
        default:
        case MassUnits_T::slugs:
            return value;
            break;
        case MassUnits_T::kilograms:
            return SLUGS_PER_KILOGRAM * value;
            break;
        case MassUnits_T::pounds_mass:
            return SLUGS_PER_POUND * value;
            break;
        }
    }
}

