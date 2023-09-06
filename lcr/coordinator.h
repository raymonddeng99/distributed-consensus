#ifndef COORDINATOR_H
#define COORDINATOR_H

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
#include "node.h"
#include "server.h"

class Coordinator : public Server {
public:
    Coordinator(const char* _SOCK_PATH, int _ring_size);

    void createRing();

    void messageHandler(char* message_buffer, size_t size_message_buffer) override;

    void startLCR();

private:
    int ring_size;
    int numRounds;
    std::vector<int> heartbeats;

    void handleHeartbeat(RingMessage* message);

    void clearHeartbeats();

    bool allResponded();

    void startRound();

    struct sockaddr_un getRingAddr(int ringIdx);

    std::string getNodeAddrPath(int ringIdx);
};

#endif