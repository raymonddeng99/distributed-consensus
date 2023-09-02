#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class Process {
public:
    int id;
    bool leader;
    bool active;

    Process(int _id) : id(_id), leader(false), active(true) {}

    void sendMessage(Process& receiver) {
        cout << "Process " << id << " sends a message to Process " << receiver.id << endl;
        receiver.receiveMessage(*this);
    }

    void receiveMessage(Process& sender) {
        if (active) {
            cout << "Process " << id << " received a message from Process " << sender.id << endl;

            if (id > sender.id) {
                leader = true;
                cout << "Process " << id << " declares itself as the leader." << endl;
            }
            else {
                leader = false;
            }
            active = false;
        }
    }
};

int main() {
    int numProcesses;
    cout << "Enter the number of processes in the ring: ";
    cin >> numProcesses;

    vector<Process> processes;

    // Create processes
    for (int i = 0; i < numProcesses; ++i) {
        processes.emplace_back(i);
    }

    // Simulate message passing until a leader is elected
    srand(time(nullptr));
    int senderIdx = rand() % numProcesses;
    int receiverIdx = (senderIdx + 1) % numProcesses;

    while (true) {
        processes[senderIdx].sendMessage(processes[receiverIdx]);

        if (processes[receiverIdx].leader) {
            cout << "Leader elected: Process " << receiverIdx << endl;
            break;
        }

        senderIdx = receiverIdx;
        receiverIdx = (receiverIdx + 1) % numProcesses;
    }

    return 0;
}