#ifndef PASSENGER_H
#define PASSENGER_H

#include "Request.h"

// Represents a passenger travelling through the system.
// Wraps a Request and tracks timestamps used for statistics.
class Passenger {
public:
    int id;
    Request request;
    int requestTick;   // when the passenger asked for a lift
    int pickupTick;    // when the passenger was actually picked up
    int dropTick;       // when the passenger reached destination

    Passenger(int id, const Request& request)
        : id(id), request(request), requestTick(request.arrivalTick),
          pickupTick(-1), dropTick(-1) {}

    int waitingTime() const {
        return (pickupTick >= 0) ? (pickupTick - requestTick) : -1;
    }

    int travelTime() const {
        return (dropTick >= 0 && pickupTick >= 0) ? (dropTick - pickupTick) : -1;
    }

    int sourceFloor() const { return request.sourceFloor; }
    int destFloor() const { return request.destFloor; }
};

#endif // PASSENGER_H
