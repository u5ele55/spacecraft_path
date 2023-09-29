#include <iostream>
#include <fstream>
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
    const double JD = 2460206.883;
    const double unixTimestamp = (JD - 2440587.5) * 86400.0;
    
    Vector currentTime(7);
    //ecef - 7144843.808, 217687.110, -506463.296        562.650611, -1616.516697, 7358.157263
    //eci - 2937656.611, 14432705.729, -20836304.223        -2408.799, 2723.781, 1545.981
    Vector initialPosition = {2937656.611, 14432705.729, -20836304.223};
    Vector initialSpeed = {-2408.799, 2723.781, 1545.981};

    auto *system = new SpacecraftECI(
        Constants::Earth::MASS * Constants::Common::G, 
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed);
    RK4Solver solver(system); 

    
    std::ofstream trajectoryStream("trajectory.txt");
    std::ofstream radiotelescopesStream("telescopes.txt");
    
    Output::RadioVisibilityZones rvz("radiozones.txt");
    Output::RadioStationsDesignations rsd("designations.txt");
    
    // output radiotelescopes coords
    TelescopeCreator rdtCreator("../radiotelescopes.txt");
    std::vector<RadioTelescope> rdts = rdtCreator.create();
    RadioTelescopeSystem radioSystem(rdts);
    
    radiotelescopesStream << rdts.size() << '\n';
    
    for (int i = 0; i < rdts.size(); i ++) {
        radiotelescopesStream << blh2ecef( rdts[i].getBLH() ) << '\n';
    }

    double rotateMatrix[3][3];
    
    double step = 100;
    for (int i = 0; i < 20000; i += step) {
        double time = i;
        auto state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + unixTimestamp;

        currentTime = secsToTime(t);
        Vector ttut = TTUT(currentTime);
        
        iauC2t06a(ttut[0], ttut[1], ttut[2], ttut[3], 0, 0, rotateMatrix);
        Vector ecef = {
            x * rotateMatrix[0][0] + y * rotateMatrix[1][0] + z * rotateMatrix[2][0],
            x * rotateMatrix[0][1] + y * rotateMatrix[1][1] + z * rotateMatrix[2][1],
            x * rotateMatrix[0][2] + y * rotateMatrix[1][2] + z * rotateMatrix[2][2]
        };
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