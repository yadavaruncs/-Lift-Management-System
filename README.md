# Lift Management System Simulator (C++)

A console-based elevator (lift) simulator built in C++ to demonstrate object-oriented programming, STL usage, and the Strategy Design Pattern through interchangeable scheduling algorithms.

## Technologies Used

- C++17
- Standard Template Library (STL)
- Object-Oriented Programming (OOP)
- CMake
- Strategy Design Pattern

## Features

- Configurable building with multiple floors and elevators.
- Adjustable elevator capacity.
- Passenger requests with source and destination validation.
- Elevator operations including movement, door control, passenger pickup, and drop-off.
- Two interchangeable scheduling strategies:
  - First Come First Serve (FCFS)
  - Nearest Elevator (Nearest Car)
- Scheduling algorithms can be swapped using the Strategy design pattern without modifying the core simulation workflow.
- Simulation statistics including total requests served, average waiting time, floors travelled, and per-elevator utilization.

## Project Structure

```
include/
├── Elevator.h
├── Passenger.h
├── Request.h
├── Scheduler.h
└── Building.h
```

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Run the generated executable.

## Learning Outcomes

This project demonstrates:

- Object-Oriented Design
- Strategy Design Pattern
- Runtime Polymorphism
- STL Containers
- Modular Software Architecture
- Simulation-Based Problem Solving

## Current Limitations

- Elevators are simulated sequentially rather than concurrently.
- Passenger movement is simulated in discrete steps.
- Waiting time is measured using simulation ticks rather than real time.

## Future Improvements

- Concurrent elevator movement.
- Priority queues for emergency requests.
- Internal destination optimization.
- GUI visualization.
- Multi-threaded simulation.
- Energy-efficient scheduling algorithms.

> **Note:** If your implementation executes only one scheduling algorithm per run, update any comparison claims accordingly.
