#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unordered_map>

#define COORDINATOR_SOCK_PATH "unix_sock_coordinator"
#define INIT_SOCK_PATH "unix_sock.0"

class Process {
public:
    int id;
    bool leader;
    bool active;
    int node_sock;
    char buf[256];
    int rc;
    struct sockaddr_un node_addr;

    Process(int _id, const char* SOCK_PATH) : id(_id), leader(false), active(true) {
        node_sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (node_sock == -1){
            printf("Opening socket failed!\n");
            exit(1);
        }

        // memset(&node_addr, 0, sizeof(node_addr));
        // node_addr.sun_family = AF_UNIX;
        // strcpy(node_addr.sun_path, SOCK_PATH);
        // int len = sizeof(node_addr);
        // unlink(SOCK_PATH);

        // rc = bind(node_sock, (struct sockaddr *)&node_addr, len);
        // if (rc == -1){
        //     printf("NODE %d: Node bind error: %s\n", _id, strerror(errno));
        //     close(node_sock);
        //     exit(1);
        // }

        // rc = listen(node_sock, 1);
        // if (rc == -1){
        //     printf("NODE %d: Listen error: %s\n", _id, strerror(errno));
        //     close(node_sock);
        //     exit(1);
        // }

        printf("NODE %d listening...\n", _id);

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "HELLO FROM CLIENT");
        sendMessage(buf, strlen(buf));
    }

    void sendMessage(char* messageBuf, size_t size) {
        struct sockaddr_un coordinator_addr;
        memset(&coordinator_addr, 0, sizeof(coordinator_addr));
        coordinator_addr.sun_family = AF_UNIX;
        strcpy(coordinator_addr.sun_path, COORDINATOR_SOCK_PATH);

        rc = connect(node_sock, (struct sockaddr*)&coordinator_addr, sizeof(node_addr));
        if(rc == -1)
        {
            printf("NODE %d: Connect error. %s\n", id, strerror(errno));
            close(node_sock);
            exit(1);
        }
        printf("NODE %d: Connected to server.\n", id);

        rc = send(node_sock, messageBuf, sizeof(messageBuf), 0);
        if (rc == -1)
        {
            printf("CLIENT: Send error. %s\n", strerror(errno));
            close(node_sock);
            exit(1);
        }
        printf("CLIENT: Sent a message to server.\n");
    }

    void receiveMessage(Process& sender) {
        struct sockaddr_un client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        int len = sizeof(client_addr);

        int client_fd = accept(node_sock, (struct sockaddr *) &client_addr, (socklen_t*)&len);
        if (client_fd == -1){
            printf("NODE %d: Accept error: %s\n", id, strerror(errno));
            close(node_sock);
            close(client_fd);
            exit(1);
        }

        printf("NODE %d: Connected to client at: %s\n", id, client_addr.sun_path);
        printf("NODE %d: Waiting for message...\n", id);

        char buf[256];
        memset(buf, 0, 256);

        // while(1){
            int byte_recv = recv(client_fd, buf, sizeof(buf), 0);
            if (byte_recv == -1){
                printf("NODE %d: Error when receiving message: %s\n", id, strerror(errno));
                close(node_sock);
                close(client_fd);
                exit(1);
            }
            else {
                printf("NODE %d: Node received message: %s\n", id, buf);
            }
        // }
    }
};

class Coordinator{
public:
    int numProcesses;
    int coordinator_sock;
    Coordinator(int _numProcesses): numProcesses(_numProcesses){
        coordinator_sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (coordinator_sock == -1){
            printf("COORDINATOR: Error when opening socket.\n");
            exit(1);
        }

        struct sockaddr_un coordinator_addr;
        memset(&coordinator_addr, 0, sizeof(coordinator_addr));
        coordinator_addr.sun_family = AF_UNIX;
        strcpy(coordinator_addr.sun_path, COORDINATOR_SOCK_PATH);
        unlink(COORDINATOR_SOCK_PATH);
        int len = sizeof(coordinator_addr);

        int rc = bind(coordinator_sock, (struct sockaddr *)&coordinator_addr, len);
        if (rc == -1){
            printf("COORDINATOR: Bind error: %s\n", strerror(errno));
            close(coordinator_sock);
            exit(1);
        }

        rc = listen(coordinator_sock, numProcesses);
        if (rc == -1)
        {
            printf("COORDINATOR: Listen error: %s\n", strerror(errno));
            close(coordinator_sock);
            exit(1);
        }

        printf("COORDINATOR: Socket listening...\n");

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
                exit(0);
            }
        }

        while(true){
            receiveMessage();
        }
        
    }

    void receiveMessage(){
        struct sockaddr_un client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        int len = sizeof(client_addr);

        int client_fd = accept(coordinator_sock, (struct sockaddr *) &client_addr, (socklen_t*)&len);
        if (client_fd == -1)
        {
            printf("COORDINATOR: Accept error: %s\n", strerror(errno));
            close(coordinator_sock);
            close(client_fd);
            exit(1);
        }
        printf("SERVER: Connected to client at: %s\n", client_addr.sun_path);
        printf("SERVER: Wating for message...\n");

        memset(buf, 0, 256);
        int byte_recv = recv(client_fd, buf, sizeof(buf), 0);
        if (byte_recv == -1)
        {
            printf("SERVER: Error when receiving message: %s\n", strerror(errno));
            close(coordinator_sock);
            close(client_fd);
            exit(1);
        }
        else{
            printf("SERVER: Server received message: %s.\n", buf);
        }
    }
private:
    char buf[256];
};

int main() {
    int numProcesses;
    std::cout << "Enter the number of processes in the ring: ";
    std::cin >> numProcesses;

    Coordinator coordinator(numProcesses);

    return 0;
}