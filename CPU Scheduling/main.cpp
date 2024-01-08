#include "Process.cpp"
#include "Scheduler.cpp"

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    Scheduler scheduler(n);

    for(int i = 0; i < n; i++) {
        float at, bt;
        int pr;
        cout << "Enter process " << i + 1 << " Arrival Time, Burst Time and Priority: ";
        cin >> at >> bt >> pr;
        Process p(i + 1, at, bt, pr);
        scheduler.addProcess(p);
    }
    cout << endl;
    
    // scheduler.fcfs();
    // scheduler.sjf_nonpreemptive();
    // scheduler.sjf_preemptive();
    // scheduler.priority_nonpreemptive(); // Lower # gets higher priority
    // scheduler.priority_preemptive(); // Lower # gets higher priority
    scheduler.roundrobin(2);

    return 0;
}