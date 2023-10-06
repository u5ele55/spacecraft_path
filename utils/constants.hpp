#pragma once

#include <cmath>

namespace Constants
{
    namespace Earth
    {
        // ПЗ - 90.11
        // 2pi / T (T = 24hrs = 86400secs)
        const double ANGULAR_SPEED = 7.292115e-5; // rad/sec
        const double MASS = 5.9742e24;
        const double GEOCENTRIC_GRAVITATION_CONSTANT = 398600.4418e9;
        const double MAJOR_AXIS = 6378136; // in meters
        const double MINOR_AXIS = 6356752.3142; // in meters
    } // namespace Earth
    namespace Common
    {
        const double G = 6.67430e-11;
        const double SECONDS_IN_DAY = 86400;
    } // namespace Common
    
} // namespace Constants
