#pragma once

#include "../utils/Vector.hpp"
#include "../spacecraft_motion/modeling/AbstractSolver.hpp"
#include "../radiotelescope/RadioTelescopeSystem.hpp"
#include <vector>
#include <string>
#include <fstream>

namespace Output
{
    class RadioVisibilityZones {
    enum class Status {
        InRadiozone, InVisibilityZone, Out
    };
    struct state {
        double time;
        Status status;
    };
    public:
        RadioVisibilityZones(
            const std::string &filename, 
            AbstractSolver &solver,
            RadioTelescopeSystem &radioSystem,
            double startUnixTimestamp, 
            int stationsQuantity,
            double step);
        ~RadioVisibilityZones();
        void recordTelescope(int index, double time, int designationSize);
    private:
        AbstractSolver &solver;
        RadioTelescopeSystem &radioSystem;
        std::ofstream file;
        std::vector<std::vector<state>> statuses;
        double startUnixTimestamp;
        double step;
    private:
        void output(double time, Status status, int index);
        double bSearch(double left, double right, int index, int negative, int positive);
    };
    
} // namespace Output
