#include <iostream>
#include <fstream>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"

#include "spacecraft_motion/system/SpacecraftGreenwichCS.hpp"
#include "spacecraft_motion/system/SpacecraftECI.hpp"
#include "spacecraft_motion/modeling/RK4Solver.hpp"

int main() {
    //ecef - 7144843.808, 217687.110, -506463.296        562.650611, -1616.516697, 7358.157263
    //eci - 2937656.611, 14432705.729, -20836304.223        -2408.799, 2723.781, 1545.981
    Vector initialPosition = {2937656.611, 14432705.729, -20836304.223};
    Vector initialSpeed = {-2408.799, 2723.781, 1545.981};

    auto *system = new SpacecraftGreenwichCS(
        Constants::Earth::MASS * Constants::Common::G, 
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed);

    RK4Solver solver(system);
    
    std::ofstream stream("out.txt");

    // sofa: eci -> ecef | use c2t06a 
    for (int i = 0; i < 40000; i ++) {
        double time = i;
        auto state = solver.solve(time);
        stream << state[1] << " " << state[3] << " " << state[5] << '\n';
    }

    delete system;

    return 0;
}