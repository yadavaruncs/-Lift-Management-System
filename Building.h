#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <queue>
#include <memory>
#include <string>
#include "Elevator.h"
#include "Request.h"
#include "Passenger.h"
#include "Scheduler.h"

// Aggregated statistics captured for a single simulation run
// (i.e. for one scheduling strategy).
struct SimulationStats {
    std::string strategyName;
    int totalRequests = 0;
    int requestsServed = 0;
    long totalWaitingTime = 0;
    long totalFloorsMoved = 0;
    long totalBusyTicks = 0;
    long totalTicks = 0; // total ticks elapsed across the whole simulation

    double averageWaitingTime() const {
        return requestsServed == 0 ? 0.0 : static_cast<double>(totalWaitingTime) / requestsServed;
    }
};

// --------------------------------------------------------------------------
// The Building class owns the floors and the fleet of elevators. It accepts
// passenger requests, delegates elevator selection to whichever Scheduler
// strategy is currently plugged in, and drives each request through to
// completion while recording statistics. This is the "Context" in the
// Strategy Design Pattern.
// --------------------------------------------------------------------------
class Building {
private:
    int numFloors;
    std::vector<Elevator> elevators;
    std::queue<Request> pendingRequests;
    std::vector<Request> requestLog; // full history, used to replay across strategies
    std::unique_ptr<Scheduler> scheduler;
    int elevatorCapacity;
    int startFloorForElevators;

    int globalTick;           // simple simulation clock
    int requestCounter;

    SimulationStats stats;

    bool isValidFloor(int floor) const;

public:
    Building(int numFloors, int numElevators, int elevatorCapacity);

    void setScheduler(std::unique_ptr<Scheduler> newScheduler);

    // Queues a new passenger request. Validates the floors first.
    bool requestLift(int sourceFloor, int destFloor);

    // Processes every queued request to completion, printing the
    // step-by-step narration described in the project spec.
    void runSimulation();

    void resetForNewRun(); // resets elevators/stats but keeps configuration

    void printBuildingStatus() const;
    void printSummary() const;

    const SimulationStats& getStats() const { return stats; }
    int getNumElevators() const { return static_cast<int>(elevators.size()); }
};

#endif // BUILDING_H
