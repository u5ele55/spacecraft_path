#include "RadioTelescope.hpp"

Vector RadioTelescope::getBLH() const
{
    return blh;
}

double RadioTelescope::getAngle() const
{ 
    return angle; 
}

bool RadioTelescope::inSightseeing(const Vector &otherBLH)
{
    double B = otherBLH[0], L = otherBLH[1], H = otherBLH[2];

     

    return false;
}     