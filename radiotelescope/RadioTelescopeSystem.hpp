#pragma once

#include "RadioTelescope.hpp"
#include "../output/RadioVisibilityZones.hpp"

#include <vector>

class RadioTelescopeSystem {
public:
    RadioTelescopeSystem(std::vector<RadioTelescope> radiotelescopes);
    std::vector<Vector> targetTelescopes(Vector ecef);
private:
    std::vector<RadioTelescope> rdts;
    std::vector<Vector> rdtsECEF;
private:
    double calculateAzimuth(Vector r_sat, Vector r_st);
};