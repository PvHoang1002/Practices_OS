#include <iostream>
#include <vector>

using namespace std;

const int P = 5;
const int R = 3;

void calculateNeed(int need[P][R], int max[P][R], int allocation[P][R]) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(int available[R], int need[P][R], int max[P][R], int allocation[P][R]) {
	bool finish[P] = {0};
    int work[R];
	for (int i = 0; i < R ; i++) {
		work[i] = available[i];
    }

	int count = 0;
	while (count < P)
	{
		bool found = false;
		for (int p = 0; p < P; p++)
		{
			if (finish[p] == 0)
			{
				int j;
				for (j = 0; j < R; j++) {
                    if (need[p][j] > work[j]) {
						break;
                    }
                }

				if (j == R)
				{
					for (int k = 0 ; k < R ; k++) {
						work[k] += allocation[p][k];
                    }
                    count++;
					finish[p] = true;
					found = true;
				}
			}
		}

		if (!found)
		{
			// cout << "System is not in safe state!\n";
			return false;
		}
	}

	// cout << "System is in safe state!\n";
	return true;
}

void findSafeSeq(int available[R], int need[P][R], int allocation[P][R], bool finish[P], vector<int>& safeSeq) {
    bool found = false;
    for (int p = 0; p < P; p++) {
        if (!finish[p]) {
            int j;
            for (j = 0; j < R; j++) {
                if (need[p][j] > available[j]) {
                    break;
                }
            }

            if (j == R) {
                for (int k = 0; k < R; k++) {
                    available[k] += allocation[p][k];
                }
                safeSeq.push_back(p);
                finish[p] = true;

                findSafeSeq(available, need, allocation, finish, safeSeq);

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

void checkRequest(int available[R], int max[P][R], int allocation[P][R], int request[R], int p_request) {
    int need[P][R];
	calculateNeed(need, max, allocation);

    for (int i = 0; i < R; i++) {
        if(need[p_request][i] < request[i]) {
            cout << "Process " << p_request <<  " uses more than the declared maximum resources!\n";
            return;
        } else {
            need[p_request][i] -= request[i];
        }
    }

    for (int i = 0; i < R; i++) {
        if(available[i] < request[i]) {
            cout << "Resources are not available!\n";
            return;
        }
    }

    int tmp_available[R];
    for (int i = 0; i < R; i++) {
        tmp_available[i] = available[i] - request[i];
    }

    int tmp_allocation[P][R];
    for (int i = 0; i < P; i++) {
        for(int j = 0; j < R; j++) {
            if(i == p_request) {
                tmp_allocation[i][j] = allocation[i][j] + request[j];
            } else {
                tmp_allocation[i][j] = allocation[i][j];
            }
        }
    }

    if (isSafe(tmp_available, need, max, tmp_allocation)) {
        cout << "Request (";
        for(int i = 0; i < R - 1; i++) {
            cout << request[i] << " ";
        }
        cout << request[R - 1] << ") of P" << p_request << " is accepted!\n";

        bool finish[P] = {0};
        vector<int> safeSeq;
        findSafeSeq(tmp_available, need, tmp_allocation, finish, safeSeq);
    } else {
        cout << "Request (";
        for(int i = 0; i < R - 1; i++) {
            cout << request[i] << " ";
        }
        cout << request[R - 1] << ") of P" << p_request << " is not accepted!\n";
    }
}

int main() {
    int available[R] = {3, 3, 2};

    int max[P][R] = {{7, 5, 3}, 
                    {3, 2, 2}, 
                    {9, 0, 2}, 
                    {2, 2, 2}, 
                    {4, 3, 3}};

    int allocation[P][R] = {{0, 1, 0}, 
                            {2, 0, 0}, 
                            {3, 0, 2}, 
                            {2, 1, 1}, 
                            {0, 0, 2}};

    // int available[R] = {2, 3, 0};
    // int max[P][R] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    // int allocation[P][R] = {{0, 1, 0}, {3, 0, 2}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};

    // int need[P][R];
    // calculateNeed(need, max, allocation);

    // bool finish[P] = {0};
    // vector<int> safeSeq;

    // if (isSafe(available, need, max, allocation)) {
    //     findSafeSeq(available, need, allocation, finish, safeSeq);
    // }

    int p_request = 1;
    int request[R] = {1, 0, 2};
    checkRequest(available, max, allocation, request, p_request);

    return 0;
}
