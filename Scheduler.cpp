#include "Scheduler.h"
#include <climits>
#include <cmath>

int FCFSScheduler::selectElevator(const std::vector<Elevator>& elevators, const Request& /*req*/) {
    if (elevators.empty()) return -1;
    int chosen = nextIndex % static_cast<int>(elevators.size());
    nextIndex = (nextIndex + 1) % static_cast<int>(elevators.size());
    return chosen;
}

int NearestCarScheduler::selectElevator(const std::vector<Elevator>& elevators, const Request& req) {
    if (elevators.empty()) return -1;

    int bestIndex = -1;
    int bestDistance = INT_MAX;

    for (size_t i = 0; i < elevators.size(); ++i) {
        int distance = std::abs(elevators[i].getCurrentFloor() - req.sourceFloor);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestIndex = static_cast<int>(i);
        } else if (distance == bestDistance && bestIndex != -1) {
            // Tie-break: prefer an idle elevator over a busy one.
            bool candidateIdle = (elevators[i].getState() == ElevatorState::IDLE);
            bool currentBestIdle = (elevators[bestIndex].getState() == ElevatorState::IDLE);
            if (candidateIdle && !currentBestIdle) {
                bestIndex = static_cast<int>(i);
            }
        }
    }
    return bestIndex;
}
