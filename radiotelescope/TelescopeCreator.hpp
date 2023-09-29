#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "RadioTelescope.hpp"

class TelescopeCreator {
public:
    TelescopeCreator(const std::string &filename, bool inRadians = false);
    ~TelescopeCreator();
    std::vector<RadioTelescope> create();
private:
    std::ifstream file;
    bool inRadians;
};