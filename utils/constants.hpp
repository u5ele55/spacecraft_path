#pragma once

#include <cmath>

namespace Constants
{
    namespace Earth
    {
        const double ANGULAR_SPEED = M_PI / 43200; // 2pi / T (T = 24hrs = 86400secs)
        const double MASS = 5.9742e24; 
    } // namespace Earth
    namespace Common
    {
        const double G = 6.67430e-11;
    } // namespace Common
    
} // namespace Constants
