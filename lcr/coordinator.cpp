#include "constants.h"
#include "node.h"
#include "server.h"
#include "coordinator.h"

Coordinator::Coordinator(const char* _SOCK_PATH,
    int _ring_size)
    :   Server(_SOCK_PATH),
        ring_size(_ring_size)
    {}

void Coordinator::createRing(){
    for (int ringIdx = 0; ringIdx < ring_size; ++ringIdx) {
        pid_t node_pid = fork();
        if (node_pid == -1) {
            std::cerr << "Fork failed!" << std::endl;
            exit(1);
        }

        if (node_pid == 0) {
            const char* nodeAddrPath = getNodeAddrPath(ringIdx);
            Node node(nodeAddrPath, ringIdx, getpid(), ring_size);
            node.sendHeartbeat();
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

void Coordinator::startLCR(){
    for (int round = 0; round < ring_size; round++)
    {
        while(!allResponded())
        {
            std::chrono::seconds duration(static_cast<int>(0.75));
            std::this_thread::sleep_for(duration);
        }
        startRound();
        clearHeartbeats();            
        
    }
}

void Coordinator::handleHeartbeat(RingMessage* message)
{
    int ringIdx = message->sourceRingID;
    heartbeats[ringIdx] = 1;
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
        sendMessage(neighbor_addr, message, strlen(message));
    }
}

struct sockaddr_un Coordinator::getRingAddr(int ringIdx){
    struct sockaddr_un ring_addr;
    memset(&ring_addr, 0, sizeof(ring_addr));
    ring_addr.sun_family = AF_UNIX;
    const char* ring_addr_path = getNodeAddrPath(ringIdx);
    strcpy(ring_addr.sun_path, ring_addr_path);
    return ring_addr;
}

const char* Coordinator::getNodeAddrPath(int ringIdx){
    std::string nodeAddrString = "unix_sock." + std::to_string(ringIdx);
    const char* nodeAddr = nodeAddrString.c_str();
    return nodeAddr;
}