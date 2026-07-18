#include "Elevator.h"
#include <iostream>
#include <cmath>

Elevator::Elevator(int id, int startFloor, int capacity)
    : id(id), currentFloor(startFloor), capacity(capacity), currentLoad(0),
      direction(Direction::IDLE), state(ElevatorState::IDLE),
      totalFloorsMoved(0), requestsServed(0), busyTicks(0) {}

std::string Elevator::directionToString() const {
    switch (direction) {
        case Direction::UP: return "UP";
        case Direction::DOWN: return "DOWN";
        default: return "IDLE";
    }
}

std::string Elevator::stateToString() const {
    switch (state) {
        case ElevatorState::MOVING: return "MOVING";
        case ElevatorState::DOORS_OPEN: return "DOORS_OPEN";
        default: return "IDLE";
    }
}

void Elevator::moveTo(int targetFloor) {
    if (targetFloor == currentFloor) {
        return; // already there
    }

    direction = (targetFloor > currentFloor) ? Direction::UP : Direction::DOWN;
    state = ElevatorState::MOVING;

    std::cout << "Elevator " << id << " moving from Floor " << currentFloor
              << " to Floor " << targetFloor << "." << std::endl;

    int steps = std::abs(targetFloor - currentFloor);
    totalFloorsMoved += steps;
    busyTicks += steps;

    currentFloor = targetFloor;
    direction = Direction::IDLE;
    state = ElevatorState::IDLE;
}

void Elevator::openDoors() {
    state = ElevatorState::DOORS_OPEN;
    busyTicks += 1; // door operations consume a tick of "busy" time
    std::cout << "Elevator " << id << " doors opening at Floor " << currentFloor << "." << std::endl;
}

void Elevator::closeDoors() {
    std::cout << "Elevator " << id << " doors closing at Floor " << currentFloor << "." << std::endl;
    state = ElevatorState::IDLE;
}

bool Elevator::pickUp(int numPassengers) {
    if (currentLoad + numPassengers > capacity) {
        std::cout << "Elevator " << id << " is full! Cannot pick up passenger." << std::endl;
        return false;
    }
    currentLoad += numPassengers;
    std::cout << "Passenger picked up by Elevator " << id
              << " at Floor " << currentFloor << "." << std::endl;
    return true;
}

void Elevator::dropOff(int numPassengers) {
    currentLoad -= numPassengers;
    if (currentLoad < 0) currentLoad = 0;
    std::cout << "Passenger dropped off by Elevator " << id
               << " at Floor " << currentFloor << "." << std::endl;
}

void Elevator::goIdle() {
    direction = Direction::IDLE;
    state = ElevatorState::IDLE;
}

void Elevator::printStatus() const {
    std::cout << "Elevator " << id << " | Floor: " << currentFloor
              << " | Direction: " << directionToString()
              << " | State: " << stateToString()
              << " | Load: " << currentLoad << "/" << capacity
              << " | Requests Served: " << requestsServed
              << std::endl;
}
