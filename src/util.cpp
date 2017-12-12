#include <cmath>
#include "util.hpp"

namespace OpenBurnUtil
{
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
    double GetMotorClassPercent(double nsec)
    {
        if (nsec < 10.0f)
        { 
            if (nsec > 5.0f ) return nsec / 10.0f; //C 
            else if (nsec > 2.5f) return nsec / 5.0f; //B
            else if (nsec > 1.25f) return nsec / 2.5f; //A
            else return 0.0f; //below A, very unlikely :)          
        }

        char designation = GetMotorClass(nsec);
        int exponent =  designation - 'D'; //D is 10 - 20, we want log2(lower limit)/10

        double minNsInClass = 10.0f * pow(2, exponent);
        return 100.0f * (nsec - minNsInClass) / minNsInClass;
    }
}
