#pragma once
#include "Process.h"

#include <vector>
using namespace std;

class Scheduler {
public:
    Scheduler();
    Scheduler(int numProcesses);

    int getNumProcesses() const;
    
    void addProcess(const Process& process);
    void printProcesses();

    void fcfs();
    void sjf_nonpreemptive();
    void sjf_preemptive();
    void priority_preemptive();
    void priority_nonpreemptive();
    void roundrobin(const float QUANTUM);
    
private:
    vector<Process> processes;
    int numProcesses;

    static bool compareByID(const Process& a, const Process& b);
    static bool compareByPriority(const Process& a, const Process& b);
    static bool compareByArrivalTime(const Process& a, const Process& b);
    static bool compareByBurstTime(const Process& a, const Process& b);

    void printAvg();
};