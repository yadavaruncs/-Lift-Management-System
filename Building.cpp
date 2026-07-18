#include "Building.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Building::Building(int numFloors, int numElevators, int elevatorCapacity)
    : numFloors(numFloors), elevatorCapacity(elevatorCapacity),
      startFloorForElevators(1), globalTick(0), requestCounter(0) {

    for (int i = 0; i < numElevators; ++i) {
        elevators.emplace_back(i + 1, startFloorForElevators, elevatorCapacity);
    }
    // Default strategy so the Building is always usable.
    scheduler = std::make_unique<FCFSScheduler>();
}

void Building::setScheduler(std::unique_ptr<Scheduler> newScheduler) {
    scheduler = std::move(newScheduler);
}

bool Building::isValidFloor(int floor) const {
    return floor >= 1 && floor <= numFloors;
}

bool Building::requestLift(int sourceFloor, int destFloor) {
    if (!isValidFloor(sourceFloor) || !isValidFloor(destFloor)) {
        std::cout << "Invalid request: Floor " << sourceFloor << " or Floor "
                  << destFloor << " does not exist in a " << numFloors
                  << "-floor building. Request rejected." << std::endl;
        return false;
    }
    if (sourceFloor == destFloor) {
        std::cout << "Invalid request: source and destination floors are the same ("
                  << sourceFloor << "). Request rejected." << std::endl;
        return false;
    }

    Request req(++requestCounter, sourceFloor, destFloor, globalTick);
    pendingRequests.push(req);
    requestLog.push_back(req);
    return true;
}

void Building::resetForNewRun() {
    int count = getNumElevators();
    elevators.clear();
    for (int i = 0; i < count; ++i) {
        elevators.emplace_back(i + 1, startFloorForElevators, elevatorCapacity);
    }

    // Replay the same sequence of requests so different strategies can be
    // compared fairly on identical input.
    std::queue<Request> empty;
    std::swap(pendingRequests, empty);
    for (const auto& r : requestLog) {
        pendingRequests.push(r);
    }

    globalTick = 0;
}

void Building::printBuildingStatus() const {
    std::cout << "\n--- Building Status (" << numFloors << " floors, "
              << elevators.size() << " elevators) ---" << std::endl;
    for (const auto& e : elevators) {
        e.printStatus();
    }
    std::cout << "-------------------------------------------------\n" << std::endl;
}

void Building::runSimulation() {
    std::cout << "\n========== Starting Simulation: " << scheduler->name()
              << " ==========\n" << std::endl;

    stats = SimulationStats();
    stats.strategyName = scheduler->name();
    stats.totalRequests = static_cast<int>(pendingRequests.size());

    while (!pendingRequests.empty()) {
        Request req = pendingRequests.front();
        pendingRequests.pop();

        std::cout << "Passenger requests lift from Floor " << req.sourceFloor
                  << " to Floor " << req.destFloor << "." << std::endl;

        int idx = scheduler->selectElevator(elevators, req);
        if (idx < 0) {
            std::cout << "No elevator available for this request!" << std::endl;
            continue;
        }

        Elevator& car = elevators[idx];
        std::cout << "Elevator " << car.getId() << " assigned." << std::endl;

        int waitStartFloor = car.getCurrentFloor();
        int waitDistance = std::abs(waitStartFloor - req.sourceFloor);

        // Move the elevator to the passenger's source floor.
        car.moveTo(req.sourceFloor);
        car.openDoors();
        car.pickUp(1);
        car.closeDoors();

        // Move the elevator to the passenger's destination floor.
        car.moveTo(req.destFloor);
        car.openDoors();
        car.dropOff(1);
        car.closeDoors();
        car.goIdle();

        car.incrementRequestsServed();

        std::cout << "Request completed.\n" << std::endl;

        // --- Update statistics ---
        stats.requestsServed++;
        stats.totalWaitingTime += waitDistance; // ticks elapsed before pickup
        globalTick += waitDistance + std::abs(req.destFloor - req.sourceFloor);
    }

    for (const auto& e : elevators) {
        stats.totalFloorsMoved += e.getTotalFloorsMoved();
        stats.totalBusyTicks += e.getBusyTicks();
    }
    stats.totalTicks = globalTick;

    printBuildingStatus();
}

void Building::printSummary() const {
    std::cout << "===== Simulation Summary: " << stats.strategyName << " =====" << std::endl;
    std::cout << "Total Requests       : " << stats.totalRequests << std::endl;
    std::cout << "Requests Served      : " << stats.requestsServed << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Waiting Time : " << stats.averageWaitingTime() << " floor-ticks" << std::endl;
    std::cout << "Total Floors Moved   : " << stats.totalFloorsMoved << std::endl;

    std::cout << "\nPer-Elevator Utilization:" << std::endl;
    for (const auto& e : elevators) {
        double utilization = (stats.totalTicks == 0)
            ? 0.0
            : (100.0 * static_cast<double>(e.getBusyTicks()) / static_cast<double>(stats.totalTicks));
        std::cout << "  Elevator " << e.getId()
                   << " | Requests Served: " << e.getRequestsServed()
                   << " | Floors Moved: " << e.getTotalFloorsMoved()
                   << " | Utilization: " << utilization << "%" << std::endl;
    }
    std::cout << "==================================================\n" << std::endl;
}
