#include <iostream>
#include <fstream>

#include <chrono>
#include <ctime>

#include "sofa/sofa.h"

#include "utils/Vector.hpp"
#include "utils/constants.hpp"

#include "spacecraft_motion/system/SpacecraftGreenwichCS.hpp"
#include "spacecraft_motion/system/SpacecraftECI.hpp"
#include "spacecraft_motion/modeling/RK4Solver.hpp"

Vector TTUT(Vector currentTime) {
    double delta;
    iauDat (currentTime[0], currentTime[1], currentTime[2], currentTime[6], &delta);

    Vector currentTT = {
        currentTime[0], 
        currentTime[1], 
        currentTime[2],
        0,0,0,
        currentTime[6] + (delta + 32.184) / Constants::Common::SECONDS_IN_DAY
    };
    currentTT[3] = (int)(currentTT[6] * 24);
    currentTT[4] = (currentTT[6] * 24.0 - currentTT[3]) * 60;
    currentTT[5] = (int)((currentTT[4] - (int)(currentTT[4])) * 60);
    currentTT[4] = (int)currentTT[4];

    double TT1, TT2, UT1, UT2;

    iauDtf2d("TT", currentTT[0], currentTT[1], currentTT[2], currentTT[3], currentTT[4], currentTT[5], &TT1, &TT2);
    iauDtf2d("UTC", currentTime[0], currentTime[1], currentTime[2], currentTime[3], currentTime[4], currentTime[5], &UT1, &UT2);
    double dut1 = 0.2971914;
    double UT12, UT11;
    iauUtcut1(UT1, UT2, dut1, &UT11, &UT12);

    return {TT1, TT2, UT11, UT12};
}

Vector secsToTime(long long secs) {
    using namespace std::chrono;

    system_clock::time_point tp{seconds{secs}};
    time_t tt = system_clock::to_time_t(tp);
    tm utc_tm = *gmtime(&tt);

    Vector time = {
        (double)utc_tm.tm_year + 1900,
        (double)utc_tm.tm_mon + 1,
        (double)utc_tm.tm_mday,
        (double)utc_tm.tm_hour,
        (double)utc_tm.tm_min,
        (double)utc_tm.tm_sec,
        0
    };
    time[6] = (
        (time[3] * 60 + time[4]) * 60 + time[5]
        ) / Constants::Common::SECONDS_IN_DAY;

    return time;
}

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