#pragma once

#include <cmath>

namespace Constants
{
    namespace Earth
    {
        // 2pi / T (T = 24hrs = 86400secs)
        const double ANGULAR_SPEED = M_PI / 43200; // rad/sec
        const double MASS = 5.9742e24;
        const double MAJOR_AXIS = 6378137; // in meters
        const double MINOR_AXIS = 6356752.3142; // in meters
    } // namespace Earth
    namespace Common
    {
        const double G = 6.67430e-11;
        const double SECONDS_IN_DAY = 86400;
    } // namespace Common
    
} // namespace Constants
