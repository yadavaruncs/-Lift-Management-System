#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

// Represents a single passenger request: go from sourceFloor to destFloor.
class Request {
public:
    int id;
    int sourceFloor;
    int destFloor;
    int arrivalTick;   // simulation tick when the request was made

    Request(int id, int sourceFloor, int destFloor, int arrivalTick = 0)
        : id(id), sourceFloor(sourceFloor), destFloor(destFloor), arrivalTick(arrivalTick) {}

    bool isUpward() const { return destFloor > sourceFloor; }

    friend std::ostream& operator<<(std::ostream& os, const Request& r) {
        os << "Request#" << r.id << " [Floor " << r.sourceFloor
           << " -> Floor " << r.destFloor << "]";
        return os;
    }
};

#endif // REQUEST_H
