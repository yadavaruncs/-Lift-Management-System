#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include "Building.h"
#include "Scheduler.h"

// Small helper to keep main() readable: submits a batch of sample
// passenger requests to the building.
void submitSampleRequests(Building& building) {
    struct Trip { int from; int to; };
    std::vector<Trip> trips = {
        {2, 8},
        {1, 5},
        {9, 3},
        {4, 4},   // invalid: same source and destination
        {7, 1},
        {3, 12},
        {6, 2},
        {10, 15}  // invalid: floor 15 does not exist in a 12-floor building
    };

    for (const auto& t : trips) {
        building.requestLift(t.from, t.to);
    }
}

int main() {
    const int NUM_FLOORS = 12;
    const int NUM_ELEVATORS = 3;
    const int ELEVATOR_CAPACITY = 6;

    std::cout << "############################################################" << std::endl;
    std::cout << "#              LIFT MANAGEMENT SYSTEM (C++)               #" << std::endl;
    std::cout << "############################################################" << std::endl;
    std::cout << "Building configured with " << NUM_FLOORS << " floors, "
              << NUM_ELEVATORS << " elevators (capacity " << ELEVATOR_CAPACITY
              << " each).\n" << std::endl;

    Building building(NUM_FLOORS, NUM_ELEVATORS, ELEVATOR_CAPACITY);

    std::cout << "--- Submitting passenger requests ---" << std::endl;
    submitSampleRequests(building);
    std::cout << std::endl;

    // ---------------------------------------------------------------
    // Run 1: First Come First Serve scheduling strategy.
    // ---------------------------------------------------------------
    building.setScheduler(std::make_unique<FCFSScheduler>());
    building.runSimulation();
    building.printSummary();
    SimulationStats fcfsStats = building.getStats();

    // ---------------------------------------------------------------
    // Reset elevators and replay the exact same requests using the
    // Nearest Car strategy, to allow a fair comparison (Strategy
    // Pattern in action: only the plugged-in algorithm changes).
    // ---------------------------------------------------------------
    building.resetForNewRun();
    building.setScheduler(std::make_unique<NearestCarScheduler>());
    building.runSimulation();
    building.printSummary();
    SimulationStats nearestStats = building.getStats();

    // ---------------------------------------------------------------
    // Final comparison of algorithms.
    // ---------------------------------------------------------------
    std::cout << "===== Algorithm Comparison =====" << std::endl;
    std::cout << std::left << std::setw(35) << "Strategy"
              << std::setw(12) << "Served"
              << std::setw(20) << "Avg Wait (ticks)"
              << "Total Floors Moved" << std::endl;

    auto printRow = [](const SimulationStats& s) {
        std::cout << std::left << std::setw(35) << s.strategyName
                   << std::setw(12) << s.requestsServed
                   << std::setw(20) << s.averageWaitingTime()
                   << s.totalFloorsMoved << std::endl;
    };
    printRow(fcfsStats);
    printRow(nearestStats);

    std::cout << "\nSimulation complete. Thank you for using the Lift Management System." << std::endl;
    return 0;
}
