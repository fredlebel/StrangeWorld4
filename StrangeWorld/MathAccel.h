#ifndef _MATHACCEL_H_
#define _MATHACCEL_H_

#include <math.h>
#define PI 3.14159265

class MathAccel
{
private:
    static double* cosLookup;
    static double* sinLookup;
    static double* toRadLookup;

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
    }


    // Function name   : MathAccel::toRadians
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::toRadians(double angle)
    {
        angle = fixAngle(angle);
        return toRadLookup[(int)angle];
    }


    // Function name   : MathAccel::sin
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::sin(double angle)
    {
        angle = fixAngle(angle);
        return sinLookup[(int)angle];
    }


    // Function name   : MathAccel::cos
    // Description     : 
    // Return type     : double 
    // Argument        : int angle
    inline static double MathAccel::cos(double angle)
    {
        angle = fixAngle(angle);
        return cosLookup[(int)angle];
    }


    // Function name   : MathAccel::fixAngle
    // Description     : 
    // Return type     : int 
    // Argument        : int angle
    inline static double MathAccel::fixAngle(double angle)
    {
        while (angle >= 360)
            angle -= 360;
        while (angle < 0)
            angle += 360;
        return angle;
    }


    // Function name   : MathAccel::sqrt
    // Description     : 
    // Return type     : int 
    // Argument        : int val
    inline static double MathAccel::sqrt(const double val)
    {
        return (int)::sqrt((double)val);
    }


    // Function name   : MathAccel::dist
    // Description     : 
    // Return type     : int 
    // Argument        : int x1
    // Argument        : int y1
    // Argument        : int x2
    // Argument        : int y2
    inline static double MathAccel::dist(double x1, double y1, double x2, double y2)
    {
        double dx = x2 - x1;
        double dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }
};


#endif //_MATHACCEL_H_