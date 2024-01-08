#include "Scheduler.h"

#include <bits/stdc++.h>
using namespace std;

Scheduler::Scheduler() : numProcesses(0) {

} 

Scheduler::Scheduler(int numProcesses) : numProcesses(numProcesses) {

}

int Scheduler::getNumProcesses() const {
    return numProcesses;
}

void Scheduler::addProcess(const Process& process) {
    processes.push_back(process);
    numProcesses++;
}

void Scheduler::fcfs() {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    int n = processes.size();

    for(int i = 0; i < n; i++) {
        cout << time << "\t";

        if(time < processes[i].getArrivalTime()) {
            time = processes[i].getArrivalTime();
            cout << "---\t" << time << "\t";
        }

        cout << "P" << processes[i].getID() << "\t";

        processes[i].setRespondTime(time - processes[i].getArrivalTime());
        time += processes[i].getBurstTime();
        processes[i].setCompletionTime(time - processes[i].getArrivalTime());
        processes[i].setWaitTime(processes[i].getCompletionTime() - processes[i].getBurstTime());
    }

    cout << time << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::sjf_nonpreemptive() {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    const int n = processes.size();

    queue<Process> readyQueue;
    vector<Process> v;
    for(int i = 0; i < n; i++) {
        if(processes[i].getArrivalTime() == time) {
            v.push_back(processes[i]);
        } else {
            break;
        }
    }
    sort(v.begin(), v.end(), compareByBurstTime);
    for(int i = 0; i < v.size(); i++) {
        readyQueue.push(v[i]);
    }

    int flag = v.size() - 1;

    while(!readyQueue.empty()) {
        cout << time << "\t";

        int index = -1;
        for(int i = 0; i < n; i++) {
            if(processes[i].getID() == readyQueue.front().getID()) {
                index = i;
                break;
            }
        }

        readyQueue.pop();

        cout << "P" << processes[index].getID() << "\t";

        processes[index].setRespondTime(time - processes[index].getArrivalTime());
        time += processes[index].getBurstTime();
        processes[index].setCompletionTime(time - processes[index].getArrivalTime());
        processes[index].setWaitTime(processes[index].getCompletionTime() - processes[index].getBurstTime());

        vector<Process> tmp;
        int count = 0;
        for(int i = flag + 1; i < n; i++) {
            if(processes[i].getArrivalTime() <= time) {
                tmp.push_back(processes[i]);
                count++;
            }
        }
        flag += count;

        if(tmp.empty() && readyQueue.empty() && flag < n - 1) {
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() > time) {
                    cout << time << "\t---\t";
                    time = processes[i].getArrivalTime();
                    tmp.push_back(processes[i]);
                    flag++;
                    break;
                }
            }
        }

        if(!tmp.empty()) {
            sort(tmp.begin(), tmp.end(), compareByBurstTime);
            for(int i = 0; i < tmp.size(); i++) {
                readyQueue.push(tmp[i]);        
            }
        }
    }

    cout << time << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::sjf_preemptive() {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    int n = processes.size();

    map<int, float> mapProcess;
    for(int i = 0; i < n; i++) {
        mapProcess[processes[i].getID()] = processes[i].getBurstTime();
    }

    priority_queue<Process, vector<Process>, function<bool(const Process&, const Process&)>> readyQueue(
        [&mapProcess](const Process& a, const Process& b) {
            if (mapProcess[a.getID()] == mapProcess[b.getID()]) {
                return a.getID() > b.getID();
            }
            return mapProcess[a.getID()] > mapProcess[b.getID()];
        }
    );

    for(int i = 0; i < n; i++) {
        if(processes[i].getArrivalTime() == time) {
            readyQueue.push(processes[i]);
        } else {
            break;
        }
    }

    int flag = readyQueue.size() - 1;

    while(!readyQueue.empty()) {
        cout << time << "\t";

        int id = readyQueue.top().getID();
        int index = -1;
        for(int i = 0; i < n; i++) {
            if(processes[i].getID() == id) {
                index = i;
                break;
            }
        }

        readyQueue.pop();

        cout << "P" << processes[index].getID() << "\t";

        if(processes[index].getRespondTime() < 0) {
            processes[index].setRespondTime(time - processes[index].getArrivalTime());
        }

        while(mapProcess[id] > 0) {
            mapProcess[id]--;
            time++;
            if(flag < n - 1 && processes[flag + 1].getArrivalTime() == time) {
                if(processes[flag + 1].getBurstTime() >= mapProcess[id]) {
                    readyQueue.push(processes[flag + 1]);
                    flag++;
                } else {
                    readyQueue.push(processes[flag + 1]);
                    readyQueue.push(processes[index]);
                    flag++;
                    break;
                }
            }
        }

        if(mapProcess[id] == 0) {
            processes[index].setCompletionTime(time - processes[index].getArrivalTime());
            processes[index].setWaitTime(processes[index].getCompletionTime() - processes[index].getBurstTime());
        }

        if(readyQueue.empty() && flag < n - 1) {
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() > time) {
                    cout << time << "\t---\t";
                    time = processes[i].getArrivalTime();
                    readyQueue.push(processes[i]);
                    flag++;
                    break;
                }
            }
        }
    }

    cout << time << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::priority_nonpreemptive() {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    const int n = processes.size();

    queue<Process> readyQueue;
    vector<Process> v;
    for(int i = 0; i < n; i++) {
        if(processes[i].getArrivalTime() == time) {
            v.push_back(processes[i]);
        } else {
            break;
        }
    }
    sort(v.begin(), v.end(), compareByPriority);
    for(int i = 0; i < v.size(); i++) {
        readyQueue.push(v[i]);
    }

    int flag = v.size() - 1;

    while(!readyQueue.empty()) {
        cout << time << "\t";

        int index = -1;
        for(int i = 0; i < n; i++) {
            if(processes[i].getID() == readyQueue.front().getID()) {
                index = i;
                break;
            }
        }

        readyQueue.pop();

        cout << "P" << processes[index].getID() << "\t";

        processes[index].setRespondTime(time - processes[index].getArrivalTime());
        time += processes[index].getBurstTime();
        processes[index].setCompletionTime(time - processes[index].getArrivalTime());
        processes[index].setWaitTime(processes[index].getCompletionTime() - processes[index].getBurstTime());

        vector<Process> tmp;
        int count = 0;
        for(int i = flag + 1; i < n; i++) {
            if(processes[i].getArrivalTime() <= time) {
                tmp.push_back(processes[i]);
                count++;
            }
        }
        flag += count;

        if(tmp.empty() && readyQueue.empty() && flag < n - 1) {
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() > time) {
                    cout << time << "\t---\t";
                    time = processes[i].getArrivalTime();
                    tmp.push_back(processes[i]);
                    flag++;
                    break;
                }
            }
        }

        if(!tmp.empty()) {
            sort(tmp.begin(), tmp.end(), compareByPriority);
            for(int i = 0; i < tmp.size(); i++) {
                readyQueue.push(tmp[i]);        
            }
        }
    }

    cout << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::priority_preemptive() {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    int n = processes.size();

    map<int, float> mapProcess;
    for(int i = 0; i < n; i++) {
        mapProcess[processes[i].getID()] = processes[i].getBurstTime();
    }

    priority_queue<Process, vector<Process>, function<bool(const Process&, const Process&)>> readyQueue(
        [](const Process& a, const Process& b) {
            if (a.getPriority() == b.getPriority()) {
                return a.getID() > b.getID();
            }
            return a.getPriority() > b.getPriority();
        }
    );

    for(int i = 0; i < n; i++) {
        if(processes[i].getArrivalTime() == time) {
            readyQueue.push(processes[i]);
        } else {
            break;
        }
    }

    int flag = readyQueue.size() - 1;

    while(!readyQueue.empty()) {
        cout << time << "\t";

        int id = readyQueue.top().getID();
        int index = -1;
        for(int i = 0; i < n; i++) {
            if(processes[i].getID() == id) {
                index = i;
                break;
            }
        }

        readyQueue.pop();

        cout << "P" << processes[index].getID() << "\t";

        if(processes[index].getRespondTime() < 0) {
            processes[index].setRespondTime(time - processes[index].getArrivalTime());
        }

        while(mapProcess[id] > 0) {
            mapProcess[id]--;
            time++;
            if(flag < n - 1 && processes[flag + 1].getArrivalTime() == time) {
                if(processes[flag + 1].getPriority() >= processes[index].getPriority()) {
                    readyQueue.push(processes[flag + 1]);
                    flag++;
                } else {
                    readyQueue.push(processes[flag + 1]);
                    readyQueue.push(processes[index]);
                    flag++;
                    break;
                }
            }
        }

        if(mapProcess[id] == 0) {
            processes[index].setCompletionTime(time - processes[index].getArrivalTime());
            processes[index].setWaitTime(processes[index].getCompletionTime() - processes[index].getBurstTime());
        }

        if(readyQueue.empty() && flag < n - 1) {
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() > time) {
                    cout << time << "\t---\t";
                    time = processes[i].getArrivalTime();
                    readyQueue.push(processes[i]);
                    flag++;
                    break;
                }
            }
        }
    }

    cout << time << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::roundrobin(const float QUANTUM) {
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    float time = processes[0].getArrivalTime();
    const int n = processes.size();

    map<int, float> mapProcess;
    for(int i = 0; i < n; i++) {
        mapProcess[processes[i].getID()] = processes[i].getBurstTime();
    }
    
    queue<Process> readyQueue;
    readyQueue.push(processes[0]);

    int flag = 0;

    while(!readyQueue.empty()) {
        cout << time << "\t";

        int id = readyQueue.front().getID();

        int index = -1;
        for(int i = 0; i < n; i++) {
            if(processes[i].getID() == id) {
                index = i;
                break;
            }
        }

        readyQueue.pop();

        cout << "P" << processes[index].getID() << "\t";

        if(processes[index].getRespondTime() < 0) {
            processes[index].setRespondTime(time - processes[index].getArrivalTime());
        }

        if(mapProcess[id] > QUANTUM) {
            time += QUANTUM;
            mapProcess[id] -= QUANTUM;

            int count = 0;
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() <= time) {
                    readyQueue.push(processes[i]);
                    count++;
                }
            }
            flag += count;

            readyQueue.push(processes[index]);
        } else {
            time += mapProcess[id];
            processes[index].setCompletionTime(time - processes[index].getArrivalTime());
            processes[index].setWaitTime(processes[index].getCompletionTime() - processes[index].getBurstTime());

            int count = 0;
            for(int i = flag + 1; i < n; i++) {
                if(processes[i].getArrivalTime() <= time) {
                    readyQueue.push(processes[i]);
                    count++;
                }
            }
            flag += count;

            if(readyQueue.empty()) {
                for(int i = flag + 1; i < n; i++) {
                    if(processes[i].getArrivalTime() > time) {
                        cout << time << "\t---\t";
                        time = processes[i].getArrivalTime();
                        readyQueue.push(processes[i]);
                        flag++;
                        break;
                    }
                }
            }
        }
    }

    cout << time << endl;

    sort(processes.begin(), processes.end(), compareByID);

    printProcesses();
    printAvg();
}

void Scheduler::printProcesses() {
    cout << endl;
    for(Process process : processes) {
        process.printInfo();
    }
}

void Scheduler::printAvg() {
    int n = processes.size();
    float sumWaitTime = 0, sumRespondTime = 0, sumCompletionTime = 0;
    for(int i = 0; i < n; i++) {
        sumWaitTime += processes[i].getWaitTime();
        sumRespondTime += processes[i].getRespondTime();
        sumCompletionTime += processes[i].getCompletionTime();
    }

    cout << endl;
    cout << "The average Respond time is:\t" << sumRespondTime / n 
        << "\nThe average Wait time is:\t" << sumWaitTime / n
        << "\nThe average Completion time is:\t" << sumCompletionTime / n << endl;
}

bool Scheduler::compareByID(const Process& a, const Process& b) {
    return a.getID() < b.getID();
}

bool Scheduler::compareByPriority(const Process& a, const Process& b) {
    return a.getPriority() < b.getPriority();
}

bool Scheduler::compareByArrivalTime(const Process& a, const Process& b) {
    if(a.getArrivalTime() == b.getArrivalTime()) {
        return compareByID(a, b);
    } else {
        return a.getArrivalTime() < b.getArrivalTime();
    }
}

bool Scheduler::compareByBurstTime(const Process& a, const Process& b) {
    if(a.getBurstTime() == b.getBurstTime()) {
        return compareByID(a, b);
    } else {
        return a.getBurstTime() < b.getBurstTime();
    }
}
