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
    return false;
}