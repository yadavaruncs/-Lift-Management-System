#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include "Elevator.h"
#include "Request.h"

// --------------------------------------------------------------------------
// Abstract base class (Strategy interface) for elevator scheduling
// algorithms. Concrete strategies implement selectElevator() differently,
// allowing the Building/main logic to swap algorithms without any change
// to the surrounding code (Strategy Design Pattern + Polymorphism).
// --------------------------------------------------------------------------
class Scheduler {
public:
    virtual ~Scheduler() = default;

    // Selects the index (within `elevators`) of the elevator best suited
    // to serve `req`. Must be overridden by concrete strategies.
    virtual int selectElevator(const std::vector<Elevator>& elevators, const Request& req) = 0;

    // Human readable name of the strategy, used for reporting/comparison.
    virtual std::string name() const = 0;
};

// --------------------------------------------------------------------------
// First Come First Serve (FCFS): requests are handed to elevators in a
// simple round-robin rotation, in the order elevators were registered,
// regardless of their current position. This mirrors a naive dispatcher
// that does not consider proximity at all.
// --------------------------------------------------------------------------
class FCFSScheduler : public Scheduler {
private:
    int nextIndex; // rotates across elevators as requests arrive

public:
    FCFSScheduler() : nextIndex(0) {}

    int selectElevator(const std::vector<Elevator>& elevators, const Request& req) override;
    std::string name() const override { return "First Come First Serve (FCFS)"; }
};

// --------------------------------------------------------------------------
// Nearest Car: picks the elevator whose current floor is closest to the
// request's source floor. Ties are broken by preferring an idle elevator,
// then by lowest elevator id.
// --------------------------------------------------------------------------
class NearestCarScheduler : public Scheduler {
public:
    int selectElevator(const std::vector<Elevator>& elevators, const Request& req) override;
    std::string name() const override { return "Nearest Elevator (Nearest Car)"; }
};

#endif // SCHEDULER_H
