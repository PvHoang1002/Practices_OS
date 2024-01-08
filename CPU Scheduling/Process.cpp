#include "Process.h"

Process::Process() {

}

Process::Process(int id, float arrivalTime, float burstTime, int priority) : id(id), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority) {
    waitTime = respondTime = completionTime = -1;
}

int Process::getID() const {
    return id;
}

int Process::getPriority() const {
    return priority;
}

float Process::getArrivalTime() const {
    return arrivalTime;
}

float Process::getBurstTime() const {
    return burstTime;
}

void Process::setWaitTime(float p_waitTime) {
    waitTime = p_waitTime;
}

void Process::setRespondTime(float p_respondTime) {
    respondTime = p_respondTime;
}

void Process::setCompletionTime(float p_completionTime) {
    completionTime = p_completionTime;
}

float Process::getWaitTime() const {
    return waitTime;
}

float Process::getRespondTime() const {
    return respondTime;
}

float Process::getCompletionTime() const {
    return completionTime;
}

void Process::printInfo() const {
    cout << "Process ID:\t" << id << "\tPriority:\t" << priority << "\tArrival Time:\t" << arrivalTime
            << "\tBurst Time:\t" << burstTime << "\tRespond Time:\t" << respondTime
            << "\tWait Time:\t" << waitTime << "\tCompletion Time:\t" << completionTime << endl;
}
