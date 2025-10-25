#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

static string nowTimestamp() {
    auto t = chrono::system_clock::now();
    time_t tt = chrono::system_clock::to_time_t(t);
    tm local_tm;
#if defined(_WIN32)
    localtime_s(&local_tm, &tt);
#else
    localtime_r(&tt, &local_tm);
#endif
    ostringstream oss;
    oss << put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void logService(const string &filename, const string &client) {
    ofstream ofs(filename, ios::app);
    if (!ofs) {
        cerr << "Cannot open log file: " << filename << "\n";
        return;
    }
    ofs << nowTimestamp() << " - " << client << "\n";
}

void showQueue(const queue<string> &q) {
    if (q.empty()) {
        cout << "Queue is empty.\n";
        return;
    }
    queue<string> tmp = q;
    cout << "Queue (front -> back):\n";
    size_t idx =1;
    while (!tmp.empty()) {
        cout << idx++ << ". " << tmp.front() << "\n";
        tmp.pop();
    }
}

int main() {
    queue<string> q;
    stack<string> ops;
    const string logFile = "service_log.txt";
    int nextClientId =1;

    for (int i =0; i <5; ++i) {
        q.push("Client_" + to_string(nextClientId++));
    }

    while (true) {
        cout << "\nMenu:\n";
        cout << "1) Show queue\n";
        cout << "2) Serve next client\n";
        cout << "3) Undo last serve\n";
        cout << "4) Add simulated clients\n";
        cout << "5) Exit\n";
        cout << "Choose an option: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            string dummy;
            getline(cin, dummy);
            cout << "Invalid input. Enter a number from menu.\n";
            continue;
        }

        if (choice ==1) {
            showQueue(q);
        } else if (choice ==2) {
            if (q.empty()) {
                cout << "Cannot serve: queue is empty.\n";
            } else {
                string client = q.front();
                q.pop();
                cout << "Serving: " << client << "\n";
                logService(logFile, client);
                ops.push(client);
            }
        } else if (choice ==3) {
            if (ops.empty()) {
                cout << "Nothing to undo.\n";
            } else {
                string last = ops.top();
                ops.pop();
                queue<string> newq;
                newq.push(last);
                while (!q.empty()) { newq.push(q.front()); q.pop(); }
                q.swap(newq);
                cout << "Undo: returned " << last << " to head of queue.\n";
            }
        } else if (choice ==4) {
            cout << "How many simulated clients to add? ";
            int n;
            if (!(cin >> n) || n <=0) {
                cout << "Invalid number. Must be positive.\n";
                cin.clear();
                string dummy; getline(cin, dummy);
                continue;
            }
            for (int i =0; i < n; ++i) q.push("Client_" + to_string(nextClientId++));
            cout << "Added " << n << " clients.\n";
        } else if (choice ==5) {
            cout << "Exiting.\n";
            break;
        } else {
            cout << "Unknown option.\n";
        }
    }

    return 0;
}