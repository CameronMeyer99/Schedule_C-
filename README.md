# Schedule_C-

Overview

The Scheduler program is a C++ application designed to simulate the scheduling of processes in an operating system. This simulation includes process generation, CPU scheduling, and disk I/O handling. The program aims to provide statistics such as average turnaround time, throughput, CPU and disk utilization, and average queue sizes.

Features

Process generation using Poisson distribution.
Simulated CPU and disk scheduling.
Calculation of various performance metrics.
Command-line parameter input for customization.
Requirements

C++ Compiler (e.g., GCC, Clang)
Standard C++ Library


To compile the Scheduler program, use a C++ compiler such as g++. For example:
g++ Scheduler.cpp -o Scheduler


To run the program, provide command-line arguments for lambda (arrival rate), average CPU service time, and average disk service time:
./Scheduler lambda avgCpuServiceTime avgDiskServiceTime

Classes

Process
Represents a single process with attributes like ID, arrival time, CPU service time, and disk service time.

Event
Represents an event in the simulation with attributes such as event type, event time, and associated process. Event types include ARRIVAL, CPU_DONE, and DISK_DONE.

Global Variables

completedProcesses: Tracks the number of processes that have completed.
lambda: The arrival rate of processes.
avgCpuServiceTime: Average service time for CPU.
avgDiskServiceTime: Average service time for disk.
currentTime: Current time in the simulation.
Functions

poissonDist(double lambda): Generates a random number based on Poisson distribution.
generateProcess(int id): Generates a new process.
scheduleCpu(Process* process): Schedules a process to the CPU.
scheduleDisk(Process* process): Schedules a process to the Disk.
Main Simulation Loop

The main loop of the program handles the scheduling and completion of processes, updating the current time, and recording various statistics.

Output

The program outputs several statistics, including:

Average turnaround time
Average throughput
Average CPU utilization
Average Disk utilization
Average CPU Ready queue size
Average Disk queue size
Number of Processes Generated
