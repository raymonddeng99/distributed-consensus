#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unordered_map>

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

    // vector<Process> numProcesses
    // cout << "server_addr sizeof " << sizeof(server_addr) << endl;

    unordered_map<int, sockaddr_un> ringModel;

    // Create processes
    for (int i = 0; i < numProcesses; ++i) {
        pid_t child_pid = fork();
        if (child_pid == -1) {
            cerr << "Fork failed!" << endl;
            return 1;
        }

        if (child_pid == 0) {
            cout << "I am a child!" << getpid() << endl;

            struct sockaddr_un addr;
            cout << "sockaddr_un size " << sizeof(sockaddr_un) << endl;

            int sock = socket(AF_UNIX, SOCK_STREAM, 0);
            if (sock == -1){
            	cout << "Opening socket failed!" << endl;
            	return 1;
            }

            return 0;
        }
    }

    // Simulate message passing until a leader is elected
    // srand(time(nullptr));
    // int senderIdx = rand() % numProcesses;
    // int receiverIdx = (senderIdx + 1) % numProcesses;

    // while (true) {
    //     processes[senderIdx].sendMessage(processes[receiverIdx]);

    //     if (processes[receiverIdx].leader) {
    //         cout << "Leader elected: Process " << receiverIdx << endl;
    //         break;
    //     }

    //     senderIdx = receiverIdx;
    //     receiverIdx = (receiverIdx + 1) % numProcesses;
    // }

    return 0;
}