#include "constants.h"
#include "node.h"
#include "server.h"
#include "coordinator.h"

Coordinator::Coordinator(const char* _SOCK_PATH,
    int _ring_size)
    :   Server(_SOCK_PATH),
        ring_size(_ring_size)
    {
        for (int i = 0; i < ring_size; i++)
        {
            heartbeats.push_back(0);
        }

        pid_t child_pid = fork();
        if (child_pid == -1) {
            std::cerr << "Fork failed!" << std::endl;
            exit(1);
        }
        if (child_pid == 0) {
            createRing();
            exit(0);
        }
        else{
            start();
        }
    }

void Coordinator::createRing(){
    for (int ringIdx = 0; ringIdx < ring_size; ++ringIdx) {
        pid_t node_pid = fork();
        if (node_pid == -1) {
            std::cerr << "Fork failed!" << std::endl;
            exit(1);
        }

        if (node_pid == 0) {
            std::string nodeAddrString = getNodeAddrPath(ringIdx);
            Node node(nodeAddrString.c_str(), ringIdx, ring_size);
            node.sendHeartbeat();
            node.start();
            exit(0);
        }
    }
}

void Coordinator::messageHandler(char* message_buffer, size_t size_message_buffer)
{
    RingMessage* receivedMessage = reinterpret_cast<RingMessage*>(message_buffer);

    if (size_message_buffer < sizeof(RingMessage)) {
        perror("Message handler error");
        return;
    }

    switch (receivedMessage->type) {
        case MessageType::Heartbeat:
            handleHeartbeat(receivedMessage);
            break;
        default:
            // Handle unknown message type
            break;
    }
}

void Coordinator::handleHeartbeat(RingMessage* message)
{
    int ringIdx = message->sourceRingID;
    heartbeats[ringIdx] = 1;

    if (allResponded() && numRounds < ring_size)
    {
        numRounds += 1;
        startRound();
        clearHeartbeats();
    }
}

void Coordinator::clearHeartbeats()
{
    for (int& heartbeat : heartbeats) {
        heartbeat = 0;
    }
}

bool Coordinator::allResponded()
{
    for (int heartbeat : heartbeats)
    {
        if (heartbeat == 0)
        {
            return false;
        }
    }
    return true;
}

void Coordinator::startRound(){
    for (int ring_idx = 0; ring_idx < ring_size; ring_idx++)
    {
        struct sockaddr_un neighbor_addr = getRingAddr(ring_idx);

        RingMessage startRoundMessage;
        startRoundMessage.type = MessageType::StartRound;
        startRoundMessage.sourceRingID = -1;
        startRoundMessage.messagePID = -1;

        char message[256];
        memcpy(message, &startRoundMessage, sizeof(startRoundMessage));
        sendMessage(neighbor_addr, message, sizeof(message));
    }
}

struct sockaddr_un Coordinator::getRingAddr(int ringIdx){
    struct sockaddr_un ring_addr;
    memset(&ring_addr, 0, sizeof(ring_addr));
    ring_addr.sun_family = AF_UNIX;
    std::string ring_addr_string = getNodeAddrPath(ringIdx);
    const char* ring_addr_path = ring_addr_string.c_str();
    strcpy(ring_addr.sun_path, ring_addr_path);
    return ring_addr;
}

std::string Coordinator::getNodeAddrPath(int ringIdx){
    return NODE_BASE_SOCK_PATH + std::to_string(ringIdx);
}