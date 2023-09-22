#include <iostream>
#include <fstream>

#include "sofa/sofa.h"

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"

#include "spacecraft_motion/system/SpacecraftGreenwichCS.hpp"
#include "spacecraft_motion/system/SpacecraftECI.hpp"
#include "spacecraft_motion/modeling/RK4Solver.hpp"

#include "radiotelescope/TelescopeCreator.hpp"


int main() {
    const double JD = 2460206.883;
    
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

    TelescopeCreator rdtCreator("../radiotelescopes.txt");
    std::vector<RadioTelescope> rdts = rdtCreator.create();
    std::cout << rdts[1].getBLH()<< '\n';

    std::ofstream stream("out.txt");

    double rotateMatrix[3][3];
    
    double step = 100;
    for (int i = 0; i < 40000; i += step) {
        double time = i;
        auto state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + 1695371113;

        currentTime = secsToTime(t);
        Vector ttut = TTUT(currentTime);
        
        iauC2t06a(ttut[0], ttut[1], ttut[2], ttut[3], 0, 0, rotateMatrix);
        Vector geodetic = {
            x * rotateMatrix[0][0] + y * rotateMatrix[1][0] + z * rotateMatrix[2][0],
            x * rotateMatrix[0][1] + y * rotateMatrix[1][1] + z * rotateMatrix[2][1],
            x * rotateMatrix[0][2] + y * rotateMatrix[1][2] + z * rotateMatrix[2][2]
        };
        stream << state[1] << ' ' << state[3] << ' ' << state[5] << '\n';
        stream << geodetic[0] <<" "<< geodetic[1] <<" "<< geodetic[2] << '\n';
    }

    delete system;
 
    return 0;
}