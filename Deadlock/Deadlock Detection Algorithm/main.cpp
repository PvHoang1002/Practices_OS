#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int P = 4;
const int R = 2;

void calculateRequest(vector<vector<int>>& request, vector<vector<int>>& allocation, vector<vector<int>>& max) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            request[i][j] = max[i][j] - allocation[i][j];
        }
    }
}
bool isDeadlock(vector<vector<int>>& allocation, vector<vector<int>>& request, vector<int>& available, vector<int>& deadlockedProcesses) {
    vector<bool> visited(P, false);
    vector<int> work = available;
    queue<int> q;

    for (int i = 0; i < P; i++) {
        bool canExecute = false;
        for (int j = 0; j < R; j++) {
            if (request[i][j] > available[j]) {
                canExecute = true;
                break;
            }
        }
        if (!canExecute) {
            q.push(i);
            visited[i] = true;
            for (int j = 0; j < R; j++) {
                work[j] += allocation[i][j];
            }
        }
    }

    while (!q.empty()) {
        int process = q.front();
        q.pop();

        for (int i = 0; i < P; i++) {
            if (!visited[i]) {
                bool canExecute = false;
                for (int j = 0; j < R; j++) {
                    if (request[i][j] > work[j]) {
                        canExecute = true;
                        break;
                    }
                }
                if (!canExecute) {
                    q.push(i);
                    visited[i] = true;
                    for (int j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }
                }
            }
        }
    }

    for (int i = 0; i < P; i++) {
        if (!visited[i]) {
            deadlockedProcesses.push_back(i);
        }
    }

    return !deadlockedProcesses.empty();
}

void findSafeSeq(vector<int>& available, vector<vector<int>>& request, vector<vector<int>>& allocation, vector<bool>& finish, vector<int>& safeSeq) {
    bool found = false;
    for (int p = 0; p < P; p++) {
        if (!finish[p]) {
            int j;
            for (j = 0; j < R; j++) {
                if (request[p][j] > available[j]) {
                    break;
                }
            }

            if (j == R) {
                for (int k = 0; k < R; k++) {
                    available[k] += allocation[p][k];
                }
                safeSeq.push_back(p);
                finish[p] = true;

                findSafeSeq(available, request, allocation, finish, safeSeq);

                finish[p] = false;
                safeSeq.pop_back();
                for (int k = 0; k < R; k++) {
                    available[k] -= allocation[p][k];
                }

                found = true;
            }
        }
    }

    if (!found) {
        cout << "Safe sequence: ";
        for (int i : safeSeq) {
            cout << "P" << i << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> allocation = {{1, 3}, {4, 1}, {1, 2}, {2, 0}};
    vector<vector<int>> max = {{2, 5}, {8, 4}, {2, 9}, {7, 1}};
    vector<int> available = {2, 4};

    // vector<vector<int>> allocation = {{0, 1, 0}, {2, 0, 0}, {3, 0, 3}, {2, 1, 1}, {0, 0, 2}};
    // vector<vector<int>> max = {{0, 1, 0}, {4, 0, 2}, {3, 0, 3}, {3, 1, 1}, {0, 0, 4}};
    // vector<int> available = {0, 0, 0};

    vector<vector<int>> request(P, vector<int>(R, 0));
    calculateRequest(request, allocation, max);

    vector<int> deadlockedProcesses;
    if (isDeadlock(allocation, request, available, deadlockedProcesses)) {
        cout << "System is in deadlock!" << endl;
        cout << "Deadlocked processes: ";
        for (int process : deadlockedProcesses) {
            cout << "P" << process << " ";
        }
        cout << endl;
    } else {
        cout << "System is not in deadlock!" << endl;
        vector<bool> finish(P, 0);
        vector<int> safeSeq;
        findSafeSeq(available, request, allocation, finish, safeSeq);
    }

    return 0;
}
