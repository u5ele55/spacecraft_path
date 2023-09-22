#include "coordinates.hpp"
#include "constants.hpp"


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