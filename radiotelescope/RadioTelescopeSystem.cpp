#include "RadioTelescopeSystem.hpp"

#include "../utils/coordinates.hpp"
#include "../utils/constants.hpp"

#include <cmath>
#include <iostream>

RadioTelescopeSystem::RadioTelescopeSystem(std::vector<RadioTelescope> radiotelescopes)
    : rdts(radiotelescopes)
{
    for(int i = 0; i < rdts.size(); i ++)
        rdtsECEF.push_back(blh2ecef(rdts[i].getBLH()));
}

std::vector<Vector> RadioTelescopeSystem::targetTelescopes(Vector ecef)
{
    // simplicified to sphere
    double maxDistanceSqr = ecef.dot(ecef) - Constants::Earth::MINOR_AXIS;
    std::vector<Vector> designations; // in format {r_id, dist, azimuth, angle}

    for (int rInd = 0; rInd < rdts.size(); rInd ++) {
        const auto& rtCoord = rdtsECEF[rInd];
        auto r = ecef - rtCoord;
        double distanceSqr = r.dot(r);
        if (distanceSqr < maxDistanceSqr) {
            // find angle between RT plane and vector between RT and satellite
            double cosAngle = r.dot(rtCoord) / sqrt(r.dot(r) * rtCoord.dot(rtCoord));
            // std::cout << "cos: " << cosAngle << '\n';
            if (cosAngle < 0) {
                continue;
            }
            // acos is an angle between vector and plane
            double angle = M_PI_2 - acos(cosAngle);
            if (angle < rdts[rInd].getAngle()) {
                // could be seen!
                double azimuth = calculateAzimuth(ecef, rtCoord); // TODO 
                designations.push_back({
                    (double)rInd, 
                    sqrt(distanceSqr),
                    azimuth,
                    angle
                });
            }
        }
    }
    return designations;
}

double RadioTelescopeSystem::calculateAzimuth(Vector r_sat, Vector r_st)
{
    // Normal vector to station plane
    auto normalVector = r_st * (1 / sqrt(r_st.dot(r_st)));

    double k = normalVector.dot(r_sat - r_st);

    // Projection of satellite on a station plane
    auto l_p = r_sat - normalVector*k;
    Vector north = {0,0,1};

    double cosAzimuth = l_p[2] / sqrt(l_p.dot(l_p));

    auto l_pCrossNorth = l_p.cross(north);
    double cosNormCross = l_pCrossNorth.dot(normalVector) / sqrt(l_pCrossNorth.dot(l_pCrossNorth));

    double azimuth = cosNormCross > 0 ? acos(cosAzimuth) : 2*M_PI - acos(cosAzimuth);
    std::cout << cosNormCross << " " << cosAzimuth << ' ' << azimuth << '\n';

    return azimuth;
}
