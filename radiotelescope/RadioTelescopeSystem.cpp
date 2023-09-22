#include "RadioTelescopeSystem.hpp"

#include "../utils/coordinates.hpp"
#include "../utils/constants.hpp"

#include <cmath>

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
            if (cosAngle < 0) {
                continue;
            }
            // acos is an angle between vector and plane
            double angle = M_PI_2 - acos(cosAngle);
            if (angle < rdts[rInd].getAngle()) {
                // could be seen!
                double azimuth = 0; // TODO 
                designations.push_back({
                    (double)rInd, 
                    sqrt(distanceSqr),
                    azimuth,
                    angle
                });
            }
        }
    }
    return std::vector<Vector>();
}
