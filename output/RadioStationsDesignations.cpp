#include "RadioStationsDesignations.hpp"

Output::RadioStationsDesignations::RadioStationsDesignations(const std::string &filename)
    : file(filename) {
        file << "Time\tId\tDistance\tAzimuth\tAngle\n";

}

Output::RadioStationsDesignations::~RadioStationsDesignations()
{
    file.close();
}

void Output::RadioStationsDesignations::output(double t, const Vector &designation)
{
    file << t << '\t';
    for (int i = 0; i < 3; i ++) {
        file << designation[i] << '\t';
    }
    file << designation[3] << '\n';
}
