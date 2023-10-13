#include "RadioVisibilityZones.hpp"
#include "../utils/time.hpp"
#include "../utils/coordinates.hpp"
#include <map>

Output::RadioVisibilityZones::RadioVisibilityZones(
    const std::string &filename, 
    AbstractSolver &solver, 
    RadioTelescopeSystem &radioSystem,
    double startUnixTimestamp,
    int stationsQuantity,
    double step
) 
:   file(filename), 
    solver(solver), 
    radioSystem(radioSystem),
    startUnixTimestamp(startUnixTimestamp), 
    statuses(stationsQuantity), 
    step(step)
    {
        file << "Station\tStatus\tTime\n";
    }

Output::RadioVisibilityZones::~RadioVisibilityZones()
{
    file.close();
}

void Output::RadioVisibilityZones::recordTelescope(int index, double time, int designationSize)
{
    Status status;
    switch (designationSize)
    {
    case 0:
        status = Status::Out;
        break;
    case 1:
        status = Status::InRadiozone;
        break;
    case 4:
        status = Status::InVisibilityZone;
        break;
    }
    if (statuses[index].size() == 0) {
        statuses[index].push_back({time, status});
        return;
    }

    auto prevState = statuses[index][statuses[index].size()-1];

    if (prevState.status != status) {
        statuses[index].push_back({time, status});
        output(time, status, index);
    } 
    // else {
    //     double left = time-step < 0 ? 0 : time-step;
    //     switch (status)
    //     {
    //     case Status::LeaveRadiozone:
    //         // Must enter radiozone
    //         time = bSearch(left, time, index, 0, 1);
    //         status = time ? Status::EnterRadiozone : status;
    //         break;
    //     case Status::EnterRadiozone:
    //         // Must enter visibility zone
    //         time = bSearch(left, time, index, 1, 4);
    //         status = time ? Status::EnterVisibilityZone : status;
    //         break;
    //     case Status::EnterVisibilityZone:
    //         // Must leave visibility zone
    //         time = bSearch(left, time, index, 4, 1);
    //         status = time ? Status::LeaveVisibilityZone : status;
    //         break;
    //     case Status::LeaveVisibilityZone:
    //         // Must leave radiozone
    //         time = bSearch(left, time, index, 1, 0);
    //         status = time ? Status::LeaveRadiozone : status;
    //         break;
    //     }
    // }
}

#include <iostream>

double Output::RadioVisibilityZones::bSearch(double left, double right, int index, int negative, int positive) {
    double mid;
    auto pos = solver.solve(right);
    auto ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + right));
    auto designationSize = radioSystem.targetTelescope(ecef, index).size();

    if (designationSize == negative) {
        return 0;
    }

    while (designationSize != positive) {
        mid = (right + left) / 2;
        pos = solver.solve(mid);
        ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + mid));
        designationSize = radioSystem.targetTelescope(ecef, index).size();
        if (designationSize == negative) {
            left = mid;
        } else {
            right = mid;
        }
        std::cout << mid << ' ' << right << ' ' << left << ' ' 
                  << '|' << negative << "->" << positive << " ! " << designationSize << '\n';
    }

    std::cout << "stopped 1wh " << designationSize << '=' << positive << '\n';
    pos = solver.solve(left);
    ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + left));
    designationSize = radioSystem.targetTelescope(ecef, index).size();
    std::cout << "l:" << designationSize << ' ';
    pos = solver.solve(right);
    ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + right));
    designationSize = radioSystem.targetTelescope(ecef, index).size();
    std::cout << "r:" << designationSize << ' ';
    while (right - left > 1) {
        mid = (right + left) / 2;
        pos = solver.solve(mid);
        ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + mid));
        designationSize = radioSystem.targetTelescope(ecef, index).size();
        if (designationSize == negative) {
            left = mid;
        } else if (designationSize == positive) {
            right = mid;
        } else {
            std::cout << "OOps!!!... " << negative << "->" << positive << " ! " << designationSize << '\n';
        }
    }
    return left;
}

void Output::RadioVisibilityZones::output(double time, Status status, int index)
{
    static std::map<Status, std::string> statusToStr = {
        {Status::InRadiozone, "RZ"},
        {Status::InVisibilityZone, "VZ"},
        {Status::Out, "OUT"},
    };
    Vector utc_time = unixToTime(startUnixTimestamp + time);
    file << index << "\t" << statusToStr[status] << '\t';
    for (int i = 0; i < 5; i ++) {
        file << utc_time[i] << "-";
    }
    file << utc_time[5] << "\n";
}
