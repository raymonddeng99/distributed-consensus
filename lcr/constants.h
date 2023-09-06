#ifndef CONSTANTS_H
#define CONSTANTS_H

#define COORDINATOR_SOCK_PATH "unix_sock_coordinator"
#define NODE_BASE_SOCK_PATH "unix_sock_"

enum class MessageType {
    Heartbeat,
    StartRound,
    Election,
    Leader
};

struct RingMessage {
    MessageType type;
    int sourceRingID;
    int messagePID;
};

#endif