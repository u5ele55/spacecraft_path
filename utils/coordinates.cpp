#include "coordinates.hpp"
#include "constants.hpp"
#include "time.hpp"

Vector blh2ecef(const Vector &blh)
{
    double b = blh[0], l = blh[1], h = blh[2];
    double cos_b = cos(b), cos_l = cos(l), sin_b = sin(b), sin_l = sin(l);
    
    double r = Constants::Earth::MINOR_AXIS / Constants::Earth::MAJOR_AXIS;
    double e2 = 1 - r*r;
    double N = Constants::Earth::MAJOR_AXIS / sqrt(1 - e2 * sin_l*sin_l);

    Vector ecef = {
        (N + h) * cos_l * cos_b,
        (N + h) * cos_l * sin_b,
        ((1 - e2)*N + h) * sin_l
    };
    return ecef;
}

Vector myEci2ecef(double x, double y, double z, Vector currentTime)
{
    double hrs = currentTime[3], mns = currentTime[4], scs = currentTime[5];
    double UT = (hrs * 60 + mns) * 60 + scs;
    
    long long d = dateToJd(currentTime) - 2451545;
    double t = d / 36525.0;
    
    double S_0 = 
        (6 * 60 + 41)*60 + 50.54841 
        + 8640184.812866 * t + 0.093104 * t*t 
        - 6.2e-6 * t*t*t;

    double S = S_0 + (1.00273790935 + 5.9e-11 * t) * UT;
    S *= 2 * M_PI / Constants::Common::SECONDS_IN_DAY;

    // rotation matrix for greenwich and absolute geocentric convertion
    double a = cos(S), b = sin(S);
    double X, Y, Z = z;
    
    X = a*x + b*y;
    Y = a*y - b*x;
    
    return {X, Y, Z};
}
