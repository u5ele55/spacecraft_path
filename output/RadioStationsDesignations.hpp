#pragma once

#include <string>
#include "../utils/Vector.hpp"
#include <fstream>

namespace Output
{
    class RadioStationsDesignations {
    public:
        RadioStationsDesignations(const std::string& filename);
        ~RadioStationsDesignations();
        void output(double time, const Vector& designation);
    private:
        std::ofstream file;
    };
} // namespace Output
