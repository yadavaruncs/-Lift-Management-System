#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <string>

// Direction the elevator is currently travelling in.
enum class Direction { UP, DOWN, IDLE };

// Operational state of the elevator car.
enum class ElevatorState { IDLE, MOVING, DOORS_OPEN };

// Represents a single elevator car within the building.
// Encapsulates its own position, capacity, load and movement statistics.
class Elevator {
private:
    int id;
    int currentFloor;
    int capacity;
    int currentLoad;
    Direction direction;
    ElevatorState state;

    // Statistics
    long totalFloorsMoved;
    int requestsServed;
    long busyTicks;   // ticks spent moving or with doors open (used for utilization)

public:
    Elevator(int id, int startFloor, int capacity);

    // --- Core operations ---
    void moveTo(int targetFloor);      // moves floor-by-floor towards target, printing progress
    void openDoors();
    void closeDoors();
    bool pickUp(int numPassengers = 1);   // returns false if over capacity
    void dropOff(int numPassengers = 1);
    void goIdle();

    // --- Accessors (encapsulation) ---
    int getId() const { return id; }
    int getCurrentFloor() const { return currentFloor; }
    int getCapacity() const { return capacity; }
    int getCurrentLoad() const { return currentLoad; }
    Direction getDirection() const { return direction; }
    ElevatorState getState() const { return state; }
    long getTotalFloorsMoved() const { return totalFloorsMoved; }
    int getRequestsServed() const { return requestsServed; }
    long getBusyTicks() const { return busyTicks; }

    void incrementRequestsServed() { requestsServed++; }

    std::string directionToString() const;
    std::string stateToString() const;

    void printStatus() const;
};

#endif // ELEVATOR_H
