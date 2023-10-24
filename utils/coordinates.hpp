#pragma once
#include "Vector.hpp"

Vector blh2ecef(const Vector &blh);
Vector myEci2ecef(double x, double y, double z, Vector currentTime);