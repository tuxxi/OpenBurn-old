#include <cmath>
#include "util.h"

namespace OpenBurnUtil
{
    double PoundsToSlugs(double lbs) { return 0.031081f * lbs; }   
    double PoundsToNewtons(double lbs) { return 4.44822f * lbs; } 

    char GetMotorClass(double nsec)
    {
        char designation = 0;
        if (nsec < 10.0f)
        { 
            if (nsec > 5.0f ) designation = 'C';
            else if (nsec > 2.5f) designation = 'B';
            else if (nsec > 1.25f) designation = 'A';
            else designation = 0;           
        }
        else
        {
            //All motors after 'D' are powers of two.
            //We're going to use this fact so I don't have to write a giant table
            designation = 'D'; //start with D = 10 thru 20 newton seconds, or a truncated int ALWAYS < 20
            int truncatedFloor = static_cast<int>(nsec / 10.0f); //If the motor was a D, we should get 1
            int offset = log2(truncatedFloor); //take log base 2 of our motor, if it was a D, the offset shouldbe 0.
            designation += offset; 
        }
        return designation;
    }
}
