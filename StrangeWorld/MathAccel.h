#ifndef _MATHACCEL_H_
#define _MATHACCEL_H_

#include <math.h>
#define PI 3.14159265

#define USE_SQRT_LOOKUP

class MathAccel
{
private:
    static double* cosLookup;
    static double* sinLookup;
    static double* toRadLookup;
#if defined( USE_SQRT_LOOKUP )
    static int* sqrtTableLookup;
#endif

public:
    static void MathAccel::init()
    {
        cosLookup = new double[360];
        sinLookup = new double[360];
        toRadLookup = new double[360];
        for (int i = 0; i < 360; ++i)
            toRadLookup[i] = (i * PI / 180);
        for (int i = 0; i < 360; ++i)
            cosLookup[i] = ::cos(toRadLookup[i]);
        for (int i = 0; i < 360; ++i)
            sinLookup[i] = ::sin(toRadLookup[i]);
#if defined( USE_SQRT_LOOKUP )
        sqrtTableLookup = new int[4000000];
        for (int i = 0; i < 4000000; ++i)
            sqrtTableLookup[i] = (int)::sqrt((double)i);
#endif

    }


    // Function name   : MathAccel::toRadians
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::toRadians(int angle)
    {
        angle = fixAngle(angle);
        return toRadLookup[angle];
    }


    // Function name   : MathAccel::sin
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::sin(int angle)
    {
        angle = fixAngle(angle);
        return sinLookup[angle];
    }


    // Function name   : MathAccel::cos
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::cos(int angle)
    {
        angle = fixAngle(angle);
        return cosLookup[angle];
    }


    // Function name   : MathAccel::fixAngle
    // Description     : 
    // Return type     : int 
    // Argument        : int angle
    inline static int MathAccel::fixAngle(int angle)
    {
        if (angle >= 360)
            return angle - 360;
        else if (angle < 0)
            return angle + 360;
        return angle;
    }


    // Function name   : MathAccel::sqrt
    // Description     : 
    // Return type     : int 
    // Argument        : int val
    inline static int MathAccel::sqrt(const int val)
    {
#if defined( USE_SQRT_LOOKUP )
        return sqrtTableLookup[val];
#else
        return (int)::sqrt((double)val);
#endif
    }


    // Function name   : MathAccel::dist
    // Description     : 
    // Return type     : int 
    // Argument        : int x1
    // Argument        : int y1
    // Argument        : int x2
    // Argument        : int y2
    inline static int MathAccel::dist(int x1, int y1, int x2, int y2)
    {
        int dx = x2 - x1;
        int dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }
};


#endif //_MATHACCEL_H_