#include "TelescopeCreator.hpp"

TelescopeCreator::TelescopeCreator(const std::string &filename)
    : file(filename)
{}

TelescopeCreator::~TelescopeCreator()
{
    file.close();
}

std::vector<RadioTelescope> TelescopeCreator::create()
{
    std::vector<RadioTelescope> res;

    int N;
    file >> N;
    for(int i = 0; i < N; i ++) {
        double b,l,h, angle;
        file >> b >> l >> h >> angle;
        res.emplace_back(Vector{b,l,h}, angle);
    }

    return res;
}
