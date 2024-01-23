//
//  Scheduler.cpp
//  OS Project
//
//  Created by Cameron Meyer.
//

#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>


using namespace std;

// Process class
class Process
{
public:
    int id;
    double arrivalTime;
    double cpuServiceTime;
    double diskServiceTime;
    
    // Process constructor
    Process(int ID, double Arrival_Time): id(ID), arrivalTime(Arrival_Time), cpuServiceTime(0), diskServiceTime(0) {}
};

// Event class
class Event
{
public:
    enum EventType { ARRIVAL, CPU_DONE, DISK_DONE };

    EventType type;
    double eventTime;
    Process* process;
    
    // Event constructor
    Event(EventType TYPE, double Event_Time, Process* PROCESS): type(TYPE), eventTime(Event_Time), process(PROCESS) {}
    
    // Overloading the less than operator for the priority queue
    bool operator<(const Event& other) const
    {
        return eventTime > other.eventTime;
    }
};

// My Global variables
int completedProcesses = 0;
double lambda;
double avgCpuServiceTime;
double avgDiskServiceTime;
double currentTime = 0;

priority_queue<Event> eventQueue;
queue<Process*> cpuReadyQueue;
queue<Process*> diskQueue;

double poissonDist(double lambda)
{
    double y = static_cast<double>(rand()) / static_cast<double>(RAND_MAX + 1.0);
    return ((-1 / lambda) * log(1 - y));
}

double turnaroundTimeSum = 0;
double cpuBusyTime = 0;
double diskBusyTime = 0;
double cpuQueueSizeSum = 0;
double diskQueueSizeSum = 0;
double queueChanges = 0;

// This should generate a new process
Process* generateProcess(int id)
{
    double arrivalTime = currentTime + poissonDist(lambda);
    return new Process(id, arrivalTime);
}

// Function to schedule a process to the CPU
void scheduleCpu(Process* process)
{
    if (cpuReadyQueue.empty())
    {
        double serviceTime = poissonDist(1 / avgCpuServiceTime);
        cpuBusyTime += serviceTime;
        eventQueue.push(Event(Event::CPU_DONE, currentTime + serviceTime, process));
    }
    cpuReadyQueue.push(process);
}

// Function to schedule a process to the Disk
void scheduleDisk(Process* process)
{
    if (diskQueue.empty())
    {
        double serviceTime = poissonDist(1 / avgDiskServiceTime);
        diskBusyTime += serviceTime;
        eventQueue.push(Event(Event::DISK_DONE, currentTime + serviceTime, process));
    }
    diskQueue.push(process);
}
// Check for correct number of command line arguments
int main(int arg1, char* arg2[])
{
    if (arg1 != 4)
    {
        cout << "Command line order: " << arg2[0] << " lambda, average cpu service time, average disk service time\n";
        return 1;
    }

    lambda = stod(arg2[1]);
    avgCpuServiceTime = stod(arg2[2]);
    avgDiskServiceTime = stod(arg2[3]);
    srand(time(NULL));

    

    int nextProcessId = 1;
    int totalProcessGen = 0;
    eventQueue.push(Event(Event::ARRIVAL, 0, generateProcess(nextProcessId++)));

    // Create while loop that checks for 10000 completed processes
    while (completedProcesses < 10000) {
        Event event = eventQueue.top();
        eventQueue.pop();
        currentTime = event.eventTime;

        cpuQueueSizeSum += cpuReadyQueue.size();
        diskQueueSizeSum += diskQueue.size();
        queueChanges++;

        switch (event.type)
        {
            case Event::ARRIVAL:
                scheduleCpu(event.process);
                eventQueue.push(Event(Event::ARRIVAL, currentTime + poissonDist(lambda), generateProcess(nextProcessId++)));
                totalProcessGen++;
                break;

            case Event::CPU_DONE:
                cpuReadyQueue.pop();
                if (!cpuReadyQueue.empty())
                {
                    Process* nextProcess = cpuReadyQueue.front();
                    double serviceTime = poissonDist(1.0 / avgCpuServiceTime);
                    cpuBusyTime += serviceTime;
                    eventQueue.push(Event(Event::CPU_DONE, currentTime + serviceTime, nextProcess));
                }
                
                if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) <= 0.6)
                {
                    completedProcesses++;
                    turnaroundTimeSum += currentTime - event.process->arrivalTime;
                    delete event.process;
                }
                else
                {
                    scheduleDisk(event.process);
                }
                break;

            case Event::DISK_DONE:
                diskQueue.pop();
                if (!diskQueue.empty())
                {
                    Process* nextProcess = diskQueue.front();
                    double serviceTime = poissonDist(1.0 / avgDiskServiceTime);
                    diskBusyTime += serviceTime;
                    eventQueue.push(Event(Event::DISK_DONE, currentTime + serviceTime, nextProcess));
                }
                scheduleCpu(event.process);
                break;
        }
    }

    double avgTurnaroundTime = turnaroundTimeSum / completedProcesses;
    double avgThroughput = completedProcesses / currentTime;
    double avgCpuUtilization = cpuBusyTime / currentTime;
    double avgDiskUtilization = diskBusyTime / currentTime;
    double avgCpuQueueSize = (cpuQueueSizeSum) / queueChanges;
    double avgDiskQueueSize = (diskQueueSizeSum) / queueChanges;

    cout << "Average turnaround time: " << avgTurnaroundTime << " seconds\n";
    cout << "Average throughput: " << avgThroughput << " processes per second\n";
    cout << "Average CPU utilization: " << avgCpuUtilization * 100 << "%\n";
    cout << "Average Disk utilization: " << avgDiskUtilization * 100 << "%\n";
    cout << "Average CPU Ready queue size: " << avgCpuQueueSize << "\n";
    cout << "Average Disk queue size: " << avgDiskQueueSize << "\n";
    cout << "Number of Process Generated: " << totalProcessGen << "\n";

    return 0;
}

    
    


