#include "RadioVisibilityZones.hpp"
#include "../utils/time.hpp"
#include "../utils/coordinates.hpp"

#include <iostream>
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
        file << "St\tTransition\tTime\n";
        for(int i = 0; i < stationsQuantity; i ++) {
            statuses[i] = {-1, Status::Out};
        }
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
    if (statuses[index].time == -1) {
        statuses[index] = {time, status};
        return;
    }

    auto prevState = statuses[index];

    if (prevState.status != status) {
        double left = time-step < 0 ? 0 : time-step;
        Transition transition;
        if (prevState.status == Status::Out) {
            // must be in radiozone
            if (status != Status::InRadiozone) {
                std::cout << "Radiozone is calculated incorrectly: Out->InRZ\n";
            }
            time = bSearch(left, time, index, 0, 1);
            transition = Transition::EnterRadiozone;
        }
        else if (prevState.status == Status::InRadiozone) {
            // could be out or in visibility zone
            if (status == Status::Out) {
                time = bSearch(left, time, index, 1, 0);
                transition = Transition::LeaveRadiozone;
            } else if (status == Status::InVisibilityZone) {
                time = bSearch(left, time, index, 1, 4);
                transition = Transition::EnterVisibilityZone;
            } else {
                std::cout << "Radiozone is calculated incorrectly: InRZ->smth\n";
            }
        } 
        else {
            // in visibility zone => must be in radiozone
            if (status != Status::InRadiozone) {
                std::cout << "Radiozone is calculated incorrectly: InVZ->InRZ\n";
            }
            time = bSearch(left, time, index, 4, 1);
            transition = Transition::LeaveVisibilityZone;
        }

        statuses[index] = {time, status};
        output(time, transition, index);
    } 
    
}

double Output::RadioVisibilityZones::bSearch(double left, double right, int index, int negative, int positive) {
    double mid;

    while (right - left > 1) {
        mid = (right + left) / 2;
        auto const& pos = solver.solve(mid);
        auto const& ecef = myEci2ecef(pos[1], pos[3], pos[5], unixToTime(startUnixTimestamp + mid));
        auto const& designationSize = radioSystem.targetTelescope(ecef, index).size();
        if (designationSize == negative) {
            left = mid;
        } else if (designationSize == positive) {
            right = mid;
        } else {
            std::cout << "OOps!!!... " << negative << "->" << positive << " ! " << designationSize << '\n';
            right = mid;
        }
    }
    return left;
}

void Output::RadioVisibilityZones::output(double time, Transition status, int index)
{
    static std::map<Transition, std::string> statusToStr = {
        {Transition::EnterRadiozone, "Enter RZ"},
        {Transition::EnterVisibilityZone, "Enter VZ"},
        {Transition::LeaveRadiozone, "Leave RZ"},
        {Transition::LeaveVisibilityZone, "Leave VZ"},
    };
    file << index << "\t" << statusToStr[status] << '\t';
    Vector utc_time = unixToTime(startUnixTimestamp + time);
    for (int i = 0; i < 5; i ++) {
        file << utc_time[i] << "-";
    }
    file << utc_time[5] << "\n";
}
