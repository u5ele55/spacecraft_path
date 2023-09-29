#pragma once

#include "../utils/Vector.hpp"
#include <vector>
#include <string>
#include <fstream>

namespace Output
{
    class RadioVisibilityZones {
    public:
        RadioVisibilityZones(const std::string &filename);
        ~RadioVisibilityZones();
        void output();
        void addToZone(int index);
        void setTime(const Vector& time);
    private:
        std::ofstream file;
        std::vector<int> currentZone;
        Vector currentTime;
    };
    
} // namespace Output
