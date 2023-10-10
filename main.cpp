#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "sofa/sofa.h"

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
    //ecef - 7144843.808, 217687.110, -506463.296        562.650611, -1616.516697, 7358.157263
    //eci - 2937656.611, 14432705.729, -20836304.223        -2408.799, 2723.781, 1545.981
    Vector initialPosition = {2937656.611, 14432705.729, -20836304.223};
    Vector initialSpeed = {-2408.799, 2723.781, 1545.981};

    auto *system = new SpacecraftECI(
        Constants::Earth::GEOCENTRIC_GRAVITATION_CONSTANT,
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed);
    RK4Solver solver(system, 30); 

    std::ofstream trajectoryStream("trajectory.txt");
    std::ofstream radiotelescopesStream("telescopes.txt");
    
    Output::RadioVisibilityZones rvz("radiozones.txt");
    Output::RadioStationsDesignations rsd("designations.txt");
    
    // output radiotelescopes coords
    TelescopeCreator rdtCreator("../radiotelescopes.txt");
    std::vector<RadioTelescope> rdts = rdtCreator.create();
    RadioTelescopeSystem radioSystem(rdts, true, true);
    
    radiotelescopesStream << rdts.size() << '\n';
    
    for (int i = 0; i < rdts.size(); i ++) {
        radiotelescopesStream << blh2ecef( rdts[i].getBLH() ) << '\n';
    }

    std::cout << std::fixed << std::setprecision(10) << "Start time: " << unixToTime(unixTimestamp) << '\n';
    
    double step = 100;
    int hour = 3600;
    for (int i = 0; i <= 11 * hour; i += step) {
        double time = i;
        Vector state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + unixTimestamp;

        currentTime = unixToTime(t);
        Vector ecef = myEci2ecef(x,y,z, currentTime);
        // Vector ecef = eci2ecef(x,y,z, currentTime);
        // std::cout << ecef << "=" << myEci2ecef(x,y,z, currentTime) << '\n';
        
        trajectoryStream << state[1] << ' ' << state[3] << ' ' << state[5] << '\n';
        trajectoryStream << ecef[0] <<" "<< ecef[1] <<" "<< ecef[2] << '\n';

        auto designations = radioSystem.targetTelescopes(ecef);
        if (!designations.empty()) {
            rvz.setTime(currentTime);
            for (const auto& des : designations) {
                rvz.addToZone(des[0]);
                rsd.output(i, des);
            }
            rvz.output();
        }
    }

    std::cout << "Endtime: " << currentTime << '\n';

    delete system;
 
    return 0;
}