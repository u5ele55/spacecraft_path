#include "coordinates.hpp"
#include "constants.hpp"
#include "time.hpp"
#include "sofa/sofa.h"

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

Vector eci2ecef(double x, double y, double z, Vector currentTime)
{
    static double rotateMatrix[3][3];
    
    Vector ttut = TTUT(currentTime);

    iauC2t06a(ttut[0], ttut[1], ttut[2], ttut[3], 0, 0, rotateMatrix);
    Vector ecef = {
        x * rotateMatrix[0][0] + y * rotateMatrix[1][0] + z * rotateMatrix[2][0],
        x * rotateMatrix[0][1] + y * rotateMatrix[1][1] + z * rotateMatrix[2][1],
        x * rotateMatrix[0][2] + y * rotateMatrix[1][2] + z * rotateMatrix[2][2]
    };

    return ecef;
}
