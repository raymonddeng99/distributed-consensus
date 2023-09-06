#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "constants.h"
#include "server.h"

class Node : public Server {
public:
    Node(const char* _SOCK_PATH, int _ring_id, int _ring_size);

    void sendHeartbeat();

    void messageHandler(char* message_buffer, size_t size_message_buffer) override;

private:
    int leader;
    int ring_id;
    int pid;
    int ring_size;
    std::atomic<bool> messageReceived;

    void handleElectionMessage(RingMessage* message);

    void handleLeaderMessage(RingMessage* message);

    void handleStartRoundMessage();

    struct sockaddr_un leftNeighbor();

    void sendElectionMessage(int messagePID);

    void sendLeaderMessage(int sourceRingID);
};

#endif