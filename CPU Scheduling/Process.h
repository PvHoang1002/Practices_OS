#pragma once

#include <iostream>
using namespace std;

class Process {
public:
    Process();
    Process(int id, float arrivalTime, float burstTime, int priority);
    
    int getID() const;
    int getPriority() const;
    float getArrivalTime() const;
    float getBurstTime() const;

    void setWaitTime(float waitTime);
    void setRespondTime(float respondTime);
    void setCompletionTime(float completionTime);

    float getWaitTime() const;
    float getRespondTime() const;
    float getCompletionTime() const;

    void printInfo() const;

private:
    int id;
    float arrivalTime;
    float burstTime;
    int priority;

    float waitTime;
    float respondTime;
    float completionTime;
};
