#include "RadioVisibilityZones.hpp"

Output::RadioVisibilityZones::RadioVisibilityZones(const std::string &filename)
    : file(filename), currentTime(7) {
        file << "Time\tVisible stations\n";
    }

Output::RadioVisibilityZones::~RadioVisibilityZones()
{
    file.close();
}

void Output::RadioVisibilityZones::output()
{
    for (int i = 0; i < 5; i ++) {
        file << currentTime[i] << "-";
    }
    file << currentTime[5] << "\t";
    for(int i = 0; i < currentZone.size(); i ++) {
        file << currentZone[i] << ' ';
    }
    file << '\n';

    currentZone.clear();
}

void Output::RadioVisibilityZones::addToZone(int index)
{
    currentZone.push_back(index);
}

void Output::RadioVisibilityZones::setTime(const Vector &time)
{
    currentTime = time;
}
