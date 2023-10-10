#pragma once
#include "Vector.hpp"

Vector unixToTime(long long secs);
Vector TTUT(Vector currentTime);
long long dateToUnix(Vector time);
double dateToJd(Vector date);
long long timeToSecs(Vector time);