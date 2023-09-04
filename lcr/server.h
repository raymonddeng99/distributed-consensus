#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Server {
public:
    Server(const char* _SOCK_PATH);

    void sendMessage(struct sockaddr_un target_addr, char* messageBuf, size_t size);

    void receiveMessage();

    void handleClient(int clientSocket);

    virtual void messageHandler(char* message_buffer, size_t size_message_buffer) = 0;

protected:
    int server_sock;
    const char* SOCK_PATH;

    void sigintHandler(int signal);

    void terminate();
};

#endif