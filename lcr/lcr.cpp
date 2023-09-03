#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unordered_map>

#define INIT_SOCK_PATH "unix_sock.0"

class Process {
public:
    int id;
    bool leader;
    bool active;

    Process(int _id, const char* SOCK_PATH) : id(_id), leader(false), active(true) {
        struct sockaddr_un node_addr;
        memset(&node_addr, 0, sizeof(node_addr));

        int node_sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (node_sock == -1){
            printf("Opening socket failed!\n");
            exit(1);
        }

        node_addr.sun_family = AF_UNIX;
        strcpy(node_addr.sun_path, SOCK_PATH);
        int len = sizeof(node_addr);
        unlink(SOCK_PATH);

        int rc;

        rc = bind(node_sock, (struct sockaddr *)&node_addr, len);
        if (rc == -1){
            printf("NODE %d: Node bind error: %s\n", _id, strerror(errno));
            close(node_sock);
            exit(1);
        }

        rc = listen(node_sock, 1);
        if (rc == -1){
            printf("NODE %d: Listen error: %s\n", _id, strerror(errno));
            close(node_sock);
            exit(1);
        }

        printf("NODE %d listening...\n", _id);

        struct sockaddr_un client_addr;
        memset(&client_addr, 0, sizeof(client_addr));

        int client_fd = accept(node_sock, (struct sockaddr *) &client_addr, (socklen_t*)&len);
        if (client_fd == -1){
            printf("NODE %d: Accept error: %s\n", _id, strerror(errno));
            close(node_sock);
            close(client_fd);
            exit(1);
        }

        printf("NODE %d: Connected to client at: %s\n", _id, client_addr.sun_path);
        printf("NODE %d: Waiting for message...\n", _id);

        char buf[256];
        memset(buf, 0, 256);

        // while(1){
            int byte_recv = recv(client_fd, buf, sizeof(buf), 0);
            if (byte_recv == -1){
                printf("NODE %d: Error when receiving message: %s\n", _id, strerror(errno));
                close(node_sock);
                close(client_fd);
                exit(1);
            }
            else {
                printf("NODE %d: Node received message: %s\n", _id, buf);
            }
        // }
    }

    void sendMessage(Process& receiver) {
        std::cout << "Process " << id << " sends a message to Process " << receiver.id << std::endl;
        receiver.receiveMessage(*this);
    }

    void receiveMessage(Process& sender) {
        if (active) {
            std::cout << "Process " << id << " received a message from Process " << sender.id << std::endl;

            if (id > sender.id) {
                leader = true;
                std::cout << "Process " << id << " declares itself as the leader." << std::endl;
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
    std::cout << "Enter the number of processes in the ring: ";
    std::cin >> numProcesses;

    std::unordered_map<int, sockaddr_un> ringModel;

    // Create processes
    for (int ringIdx = 0; ringIdx < numProcesses; ++ringIdx) {
        pid_t node_pid = fork();
        if (node_pid == -1) {
            std::cerr << "Fork failed!" << std::endl;
            exit(1);
        }

        if (node_pid == 0) {
            std::string SOCK_PATH_STRING = "unix_sock." + std::to_string(ringIdx);
            const char* SOCK_PATH = SOCK_PATH_STRING.c_str();
            Process node(ringIdx, SOCK_PATH);
            std::cout << "Created process with SOCK_PATH: " << SOCK_PATH << std::endl;
            exit(0);
        }
    }

    int coordinator_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (coordinator_sock == -1) {
        printf("COORDINATOR: Socket error: %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_un node_addr;
    memset(&node_addr, 0, sizeof(node_addr));
    node_addr.sun_family = AF_UNIX;
    strcpy(node_addr.sun_path, INIT_SOCK_PATH);

    int rc = connect(coordinator_sock, (struct sockaddr*)&node_addr, sizeof(node_addr));
    if(rc == -1)
    {
        printf("COORDINATOR: Connect error. %s\n", strerror(errno));
        close(coordinator_sock);
        exit(1);
    }

    printf("COORDINATOR: Connected to node %s\n", INIT_SOCK_PATH);

    for (int ringIdx = 0; ringIdx < numProcesses; ++ringIdx) {
        std::string SOCK_PATH_STRING = "unix_sock." + std::to_string(ringIdx);
        const char* SOCK_PATH = SOCK_PATH_STRING.c_str();
        remove(SOCK_PATH);
    }

    return 0;
}