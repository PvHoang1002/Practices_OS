#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

void printPages(const vector<int>& pages) {
    cout << "[";
    for(int i = 0; i < pages.size() - 1; i++) {
        cout << pages[i] << " ";
    }
    cout << pages[pages.size() - 1] << "]\n";
}

void printQueue(const queue<int>& q) {
    queue<int> tmp = q;
    cout << "Queue: [";
    int n = tmp.size();
    for(int i = 0; i < n - 1; i++) {
        cout << tmp.front() << " ";
        tmp.pop();
    }
    cout << tmp.front() << "]\n";
}

void printPriorityQueue(const priority_queue<int, vector<int>, function<bool(const int&, const int&)>> q) {
    priority_queue<int, vector<int>, function<bool(const int&, const int&)>> tmp = q;
    cout << "Queue: [";
    int n = tmp.size();
    for(int i = 0; i < n - 1; i++) {
        cout << tmp.top() << " ";
        tmp.pop();
    }
    cout << tmp.top() << "]\n";
}

void printStack(const stack<int>& s) {
    stack<int> tmp = s;
    cout << "Stack: [";
    int n = tmp.size();
    for(int i = 0; i < n - 1; i++) {
        cout << tmp.top() << " ";
        tmp.pop();
    }
    cout << tmp.top() << "]\n";
}

void removeQueue(queue<int>& q, const int& val) {
    queue<int> ref;
    int s = q.size();
    int count = 0;

    while (q.front() != val and !q.empty()) {
        ref.push(q.front());
        q.pop();
        count++;
    }

    if (q.empty()) {
        while (!ref.empty()) {
            q.push(ref.front());
            ref.pop();
        }
    } else {
        q.pop();
        while (!ref.empty()) {
            q.push(ref.front());
            ref.pop();
        }
        int k = s - count - 1;
        while (k--) {
            int p = q.front();
            q.pop();
            q.push(p);
        }
    }
}

void removeStack(stack<int>& s, const int& val) {
    stack<int> ref;

    while (!s.empty()) {
        if (s.top() != val) {
            ref.push(s.top());
        }
        s.pop();
    }

    while (!ref.empty()) {
        s.push(ref.top());
        ref.pop();
    }
}

int fifo(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages; 
    queue<int> indexes; 
 
    int pageFaults = 0; 

    for (int i = 0; i < n; i++) { 
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";
                printQueue(indexes);
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                int val = indexes.front();
                indexes.pop();

                int index = (int) (find(currentPages.begin(), currentPages.end(), val) - currentPages.begin());
                currentPages[index] = pages[i];

                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";
                printQueue(indexes);
            }
        }
    }
    return pageFaults;
}

int lru(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages; 
    queue<int> indexes; 
 
    int pageFaults = 0; 

    for (int i = 0; i < n; i++) { 
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                removeQueue(indexes, pages[i]);
                indexes.push(pages[i]);
                printQueue(indexes);
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                int val = indexes.front();
                indexes.pop();

                int index = (int) (find(currentPages.begin(), currentPages.end(), val) - currentPages.begin());
                currentPages[index] = pages[i];

                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                removeQueue(indexes, pages[i]);
                indexes.push(pages[i]);
                printQueue(indexes);
            }
        }
    }
    return pageFaults;
}

int mru(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages; 
    stack<int> indexes; 
 
    int pageFaults = 0; 

    for (int i = 0; i < n; i++) { 
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printStack(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                removeStack(indexes, pages[i]);
                indexes.push(pages[i]);
                printStack(indexes);
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                int val = indexes.top();
                indexes.pop();

                int index = (int) (find(currentPages.begin(), currentPages.end(), val) - currentPages.begin());
                currentPages[index] = pages[i];

                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printStack(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";
                removeStack(indexes, pages[i]);
                indexes.push(pages[i]);
                printStack(indexes);
            }
        }
    }
    return pageFaults;
}

int lfu(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages;
    map<int, int> freq;

    map<int, int> idx;
    for(int i = 0; i < n; i++) {
        idx[pages[i]] = -1;
    }

    priority_queue<int, vector<int>, function<bool(const int&, const int&)>> indexes (
        [&freq, &idx](const int& a, const int& b) {
            if(freq[a] == freq[b]) {
                return idx[a] > idx[b];
            }
            return freq[a] > freq[b];
        }
    );
 
    int pageFaults = 0;

    for (int i = 0; i < n; i++) {
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printPriorityQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                vector<int> tmp;
                while (!indexes.empty()) {
                    tmp.push_back(indexes.top());
                    indexes.pop();
                }

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }

                for (int page : tmp) {
                    indexes.push(page);
                }
                printPriorityQueue(indexes);
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                int val = indexes.top();
                indexes.pop();

                idx[val] = -1;
                freq[val]--;

                int index = (int) (find(currentPages.begin(), currentPages.end(), val) - currentPages.begin());
                currentPages[index] = pages[i];

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]); 
                printPriorityQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                vector<int> tmp;
                while (!indexes.empty()) {
                    tmp.push_back(indexes.top());
                    indexes.pop();
                }

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }

                for (int page : tmp) {
                    indexes.push(page);
                }
                printPriorityQueue(indexes);
            }
        }
    }
    return pageFaults;
}

int mfu(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages;
    map<int, int> freq;

    map<int, int> idx;
    for(int i = 0; i < n; i++) {
        idx[pages[i]] = -1;
    }

    priority_queue<int, vector<int>, function<bool(const int&, const int&)>> indexes (
        [&freq, &idx](const int& a, const int& b) {
            if(freq[a] == freq[b]) {
                return idx[a] > idx[b];
            }
            return freq[a] < freq[b];
        }
    );
 
    int pageFaults = 0;

    for (int i = 0; i < n; i++) {
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]);
                printPriorityQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                vector<int> temp;
                while (!indexes.empty()) {
                    temp.push_back(indexes.top());
                    indexes.pop();
                }

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }

                for (int page : temp) {
                    indexes.push(page);
                }
                printPriorityQueue(indexes);
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                int val = indexes.top();
                indexes.pop();

                idx[val] = -1;
                freq[val]--;

                int index = (int) (find(currentPages.begin(), currentPages.end(), val) - currentPages.begin());
                currentPages[index] = pages[i];

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                indexes.push(pages[i]); 
                printPriorityQueue(indexes);
            } else {
                cout << "Page " << pages[i] << ": No page fault\n";

                vector<int> temp;
                while (!indexes.empty()) {
                    temp.push_back(indexes.top());
                    indexes.pop();
                }

                freq[pages[i]]++;
                if(idx[pages[i]] == -1) {
                    idx[pages[i]] = i;
                }

                for (int page : temp) {
                    indexes.push(page);
                }
                printPriorityQueue(indexes);
            }
        }
    }
    return pageFaults;
}

int secondChance(const int pages[], const int& n, const int& frames) {
    vector<int> currentPages;
    map<int, int> refBit; 

    int pageFaults = 0;
    int pointer = 0;

    for(int i = 0; i < n; i++) { 
        if(currentPages.size() < frames) {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                currentPages.push_back(pages[i]);
                refBit[pages[i]] = 0;
                pageFaults++;

                cout << "Page " << pages[i] <<": ";
                printPages(currentPages);

                pointer = (pointer + 1) % frames;
            } else {
                if(refBit[pages[i]] == 0) {
                    refBit[pages[i]]++;
                }

                cout << "Page " << pages[i] << ": No page fault\n";
            }
        } else {
            if(find(currentPages.begin(), currentPages.end(), pages[i]) == currentPages.end()) {
                cout << "Current pointer " << pointer << " Page: " << currentPages[pointer] << " Ref bit: " << refBit[currentPages[pointer]] << endl;
                if(refBit[currentPages[pointer]] == 0) {
                    currentPages[pointer] = pages[i];

                    refBit[pages[i]] = 0;
                    pageFaults++;

                    cout << "Page " << pages[i] << ": ";
                    printPages(currentPages);

                    pointer = (pointer + 1) % frames;
                } else {
                    refBit[currentPages[pointer]] = 0;
                    for(int idx = (pointer + 1) % frames; idx < frames; idx = (idx + 1) % frames) {
                        if(refBit[currentPages[idx]] == 0) {
                            pointer = idx;
                            break;
                        }
                    }

                    currentPages[pointer] = pages[i];

                    refBit[pages[i]] = 0;
                    pageFaults++;

                    cout << "Page " << pages[i] << ": ";
                    printPages(currentPages);

                    pointer = (pointer + 1) % frames;
                }
            } else {
                if(refBit[pages[i]] == 0) {
                    refBit[pages[i]]++;
                }

                cout << "Page " << pages[i] << ": No page fault\n";
            }
        }
        cout << "Pointer " << pointer << endl;
    }
    return pageFaults;
}

int main() {
    srand(time(0));

    int frames;
    cout << "Enter the number of page frames (1 to 7): "; 
    cin >> frames;

    // const int n = 20; 
    // int pages[n]; 
    // for (int i = 0; i < n; ++i) { 
    //     pages[i] = rand() % 10; 
    // }
    // cout << "Page Reference String: ["; 
    // for (int i = 0; i < n - 1; ++i) { 
    //     cout << pages[i] << " "; 
    // }
    // cout << pages[n - 1] << "]\n";

    const int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
    int n = sizeof(pages) / sizeof(pages[0]);

    // const int pages[] = {0,1,2,3,4,5,3,1,2,0,1,2,3,4,5,0};
    // const int pages[] = {0,4,1,4,2,4,3,4,2,4,0,4,1,4,2,4,3,4};
    // const int pages[] = {2,3,2,1,5,2,4,5,3,2,3,5};
    // const int pages[] = {1,3,1,2,3,2,4};
    // const int pages[] = {1,3,3,2,1,2,4};
    // const int pages[] = {1,3,3,2,1,4,2,5}; // {1,3,3,2,1,2,4};
    // const int pages[] = {1,2,3,4,1,2,5,1,2,3,4,5}; // Belady Anomaly

    int pageFaults = 0;
    pageFaults = fifo(pages, n, frames);
    // pageFaults = lru(pages, n, frames);
    // pageFaults = mru(pages, n, frames);
    // pageFaults = lfu(pages, n, frames);
    // pageFaults = mfu(pages, n, frames);
    // pageFaults = secondChance(pages, n, frames);

    cout << "Number of page faults: " << pageFaults << "\n";

    return 0;
}