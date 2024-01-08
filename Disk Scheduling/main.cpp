#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <map>

const int DISK_SIZE = 200;

using namespace std;

int fcfs(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;
    const int n = tracks.size();
    for(int i = 0; i < n; i++) {
        cout << tmp << "\t-->\t" << tracks[i] << endl;
        moves += abs(tracks[i] - tmp);
        tmp = tracks[i];
    }

    return moves;
}

int sstf(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;
    const int n = tracks.size();

    map<int, int> visit;

    for(int i = 0; i < n; i++) {
        int minDis = INT_MAX;
        int closestTrack = 0;
        for(int j = 0; j < n; j++) {
            if(!visit[tracks[j]]) {
                if(abs(tmp - tracks[j]) <= minDis) {
                    minDis = abs(tmp - tracks[j]);
                    closestTrack = tracks[j];
                }
            }
        }
        visit[closestTrack]++;
        cout << tmp << "\t-->\t" << closestTrack << endl;
        moves += abs(closestTrack - tmp);
        tmp = closestTrack;
    }

    return moves;
}

int scan(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;

    string direction = "left";
    if(start > DISK_SIZE - 1 - start) {
        direction = "right";
    }
    vector<int> left, right;
    if(direction == "left") {
        left.push_back(0);
    } else {
        right.push_back(DISK_SIZE - 1);
    }

    int n = tracks.size();
    for (int i = 0; i < n; i++) {
        if (tracks[i] <= start)
            left.push_back(tracks[i]);
        if (tracks[i] > start)
            right.push_back(tracks[i]);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int run = 2;
    while(run--) {
        if(direction == "left") {
            for(int i = left.size() - 1; i >= 0; i--) {
                cout << tmp << "\t-->\t" << left[i] << endl;
                moves += abs(left[i] - tmp);
                tmp = left[i];
            }
            direction = "right";
        } else {
            for(int i = 0; i < right.size(); i++) {
                cout << tmp << "\t-->\t" << right[i] << endl;
                moves += abs(right[i] - tmp);
                tmp = right[i];
            }
            direction = "left";
        }
    }

    return moves;
}

int c_scan(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;

    int n = tracks.size();
    vector<int> left = {0}, right = {DISK_SIZE - 1};
    for (int i = 0; i < n; i++) {
        if (tracks[i] <= start)
            left.push_back(tracks[i]);
        if (tracks[i] > start)
            right.push_back(tracks[i]);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    string direction = "right";
    if(left.size() > right.size()) {
        direction = "left";
        sort(left.begin(), left.end(), greater<int>());
        sort(right.begin(), right.end(), greater<int>());
    }

    int run = 2;
    while(run--) {
        if(direction == "left") {
            for(int i = 0; i < left.size(); i++) {
                cout << tmp << "\t-->\t" << left[i] << endl;
                if(tmp != DISK_SIZE - 1 || left[i] != 0) {
                    moves += abs(left[i] - tmp);
                }
                tmp = left[i];      
            }
            direction = "right";
        } else {
            for(int i = 0; i < right.size(); i++) {
                cout << tmp << "\t-->\t" << right[i] << endl;
                if(tmp != 0 || right[i] != DISK_SIZE - 1) {
                    moves += abs(right[i] - tmp);
                }
                tmp = right[i];
            }
            direction = "left";
        }
    }

    return moves;
}

int look(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;

    string direction = "left";
    if(start > DISK_SIZE - 1 - start) {
        direction = "right";
    }

    vector<int> left, right;
    int n = tracks.size();
    for (int i = 0; i < n; i++) {
        if (tracks[i] <= start)
            left.push_back(tracks[i]);
        if (tracks[i] > start)
            right.push_back(tracks[i]);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int run = 2;
    while(run--) {
        if(direction == "left") {
            for(int i = left.size() - 1; i >= 0; i--) {
                cout << tmp << "\t-->\t" << left[i] << endl;
                moves += abs(left[i] - tmp);
                tmp = left[i];
            }
            direction = "right";
        } else {
            for(int i = 0; i < right.size(); i++) {
                cout << tmp << "\t-->\t" << right[i] << endl;
                moves += abs(right[i] - tmp);
                tmp = right[i];
            }
            direction = "left";
        }
    }

    return moves;
}

int c_look(const int& start, const vector<int>& tracks) {
    int tmp = start;
    int moves = 0;

    int n = tracks.size();
    vector<int> left, right;
    for (int i = 0; i < n; i++) {
        if (tracks[i] <= start)
            left.push_back(tracks[i]);
        if (tracks[i] > start)
            right.push_back(tracks[i]);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    string direction = "right";
    if(left.size() > right.size()) {
        direction = "left";
        sort(left.begin(), left.end(), greater<int>());
        sort(right.begin(), right.end(), greater<int>());
    }

    int run = 2;
    while(run--) {
        if(direction == "left") {
            for(int i = 0; i < left.size(); i++) {
                cout << tmp << "\t-->\t" << left[i] << endl;
                moves += abs(left[i] - tmp);
                tmp = left[i];      
            }
            direction = "right";
        } else {
            for(int i = 0; i < right.size(); i++) {
                cout << tmp << "\t-->\t" << right[i] << endl;
                moves += abs(right[i] - tmp);
                tmp = right[i];
            }
            direction = "left";
        }
    }

    return moves;
}

int main() {
    const vector<int> tracks = {98, 183, 37, 122, 14, 124, 65, 67};
    // const vector<int> tracks = {176, 79, 34, 60, 92, 11, 41, 114};
    const int start = 53;

    int moves = look(start, tracks);
    cout << "Move through " << moves << " tracks";

    return 0;
}