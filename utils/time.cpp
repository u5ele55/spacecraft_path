#include "time.hpp"

#include "sofa/sofa.h"
#include "constants.hpp"

#include <chrono>
#include <ctime>

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