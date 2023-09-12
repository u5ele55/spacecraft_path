#include <iostream>
#include <fstream>
#include "utils/Vector.hpp"

#include "utils/constants.hpp"

#include "spacecraft_motion/system/SpacecraftGreenwichCS.hpp"
#include "spacecraft_motion/modeling/RK4Solver.hpp"

int main() {
    Vector initialPosition = {7e6, 1, 2};
    Vector initialSpeed = {0, 100, 1};


    auto *system = new SpacecraftGreenwichCS(Constants::Earth::MASS * Constants::Common::G, Constants::Earth::ANGULAR_SPEED, initialPosition, initialSpeed);

    RK4Solver solver(system);
    
    std::ofstream stream("out.txt");

    for (int i = 0; i < 10000; i ++) {
        double time = i;
        auto state = solver.solve(time);
        stream << state[1] << " " << state[3] << " " << state[5] << '\n';
    }


    delete system;

    return 0;
}