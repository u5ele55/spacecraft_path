#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"
#include "utils/coordinates.hpp"

#include "spacecraft_motion/system/SpacecraftGreenwichCS.hpp"
#include "spacecraft_motion/system/SpacecraftECI.hpp"
#include "spacecraft_motion/modeling/RK4Solver.hpp"

#include "radiotelescope/TelescopeCreator.hpp"
#include "radiotelescope/RadioTelescopeSystem.hpp"

#include "output/RadioVisibilityZones.hpp"
#include "output/RadioStationsDesignations.hpp"

int main() {
    const double JD = 2460206.383;
    const double unixTimestamp = (JD - 2440587.5) * Constants::Common::SECONDS_IN_DAY;
    
    Vector currentTime(7);
    
    Vector initialPosition = {6871257.864, 0.0, 0.0};
    Vector initialSpeed = {0.0, 3810.1125727278977, 6599.308558521686};

    auto *system = new SpacecraftECI(
        Constants::Earth::GEOCENTRIC_GRAVITATION_CONSTANT,
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed);
    RK4Solver solver(system, 10); 

    std::ofstream trajectoryStream("trajectory.txt");
    std::ofstream radiotelescopesStream("telescopes.txt");
    
    
    // output radiotelescopes coords
    TelescopeCreator rdtCreator("../radiotelescopes.txt");
    std::vector<RadioTelescope> rdts = rdtCreator.create();
    RadioTelescopeSystem radioSystem(rdts, true, true);
    
    radiotelescopesStream << rdts.size() << '\n';
    for (int i = 0; i < rdts.size(); i ++) {
        radiotelescopesStream << blh2ecef( rdts[i].getBLH() ) << '\n';
    }

    std::cout << "Start time: " << unixToTime(unixTimestamp) << '\n';
    
    double step = 30;
    int hour = 3600;

    Output::RadioVisibilityZones rvz("radiozones.txt", solver, radioSystem, unixTimestamp, rdts.size(), step);
    Output::RadioStationsDesignations rsd("designations.txt");
    
    for (int i = 0; i <= 1.75 * hour; i += step) {
        double time = i;
        Vector state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + unixTimestamp;

        currentTime = unixToTime(t);
        Vector ecef = myEci2ecef(x,y,z, currentTime);
        
        trajectoryStream << state[1] << ' ' << state[3] << ' ' << state[5] << '\n';
        trajectoryStream << ecef[0] <<" "<< ecef[1] <<" "<< ecef[2] << '\n';

        for (int r = 0; r < rdts.size(); r ++) {
            const auto& designation = radioSystem.targetTelescope(ecef, r);
            int desSize = designation.size();
            rvz.recordTelescope(r, time, desSize);
            if (desSize == 4) {
                rsd.output(i, designation);
            }
        }
    }

    std::cout << "Endtime: " << currentTime << '\n';

    delete system;
 
    return 0;
}