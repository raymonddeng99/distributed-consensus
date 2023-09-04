#include "constants.h"
#include "node.h"


Node::Node(const char* _SOCK_PATH, 
	int _ring_id, 
	int _pid,
	int _ring_size)
	:	Server(_SOCK_PATH),
		ring_id(_ring_id),
		pid(_pid),
		ring_size(_ring_size)
	{}

void Node::sendHeartbeat()
{
    struct sockaddr_un coordinator_addr;
    memset(&coordinator_addr, 0, sizeof(coordinator_addr));
    coordinator_addr.sun_family = AF_UNIX;
    strcpy(coordinator_addr.sun_path, COORDINATOR_SOCK_PATH);

	RingMessage startMessage;
    startMessage.type = MessageType::Heartbeat;
    startMessage.sourceRingID = ring_id;
    startMessage.messagePID = -1;

    char message[256];
    memcpy(message, &startMessage, sizeof(startMessage));
	sendMessage(coordinator_addr, message, strlen(message));
}

void Node::messageHandler(char* message_buffer, size_t size_message_buffer)
{
	RingMessage* receivedMessage = reinterpret_cast<RingMessage*>(message_buffer);
	messageReceived.store(true);

	if (size_message_buffer < sizeof(RingMessage)) {
        perror("Message handler error");
        return;
    }

    switch (receivedMessage->type) {
    	case MessageType::StartRound:
    		handleStartRoundMessage();
    		break;
        case MessageType::Election:
            handleElectionMessage(receivedMessage);
            sendHeartbeat();
            break;
        case MessageType::Leader:
            handleLeaderMessage(receivedMessage);
            sendHeartbeat();
            break;
        default:
            // Handle unknown message type
            break;
    }
}

void Node::handleElectionMessage(RingMessage* message)
{
    if (message->messagePID == pid) {
        sendLeaderMessage(ring_id);
    }

    if (message->messagePID > pid) {
    	sendElectionMessage(message->messagePID);
    }
}

void Node::handleLeaderMessage(RingMessage* message)
{
    leader = message->sourceRingID;
    printf("Leader elected: %d\n", leader);
    terminate();
}

void Node::handleStartRoundMessage()
{
	// begin timeout of 0.5 seconds
	std::chrono::seconds duration(static_cast<int>(0.75));
	std::this_thread::sleep_for(duration);
	if (!messageReceived.load())
	{
		printf("Node %d received message, function should exit.\n", ring_id);
	}
	else
	{
		sendElectionMessage(pid);
	}
}

struct sockaddr_un Node::leftNeighbor()
{
	struct sockaddr_un neighbor_addr;
	memset(&neighbor_addr, 0, sizeof(neighbor_addr));
	neighbor_addr.sun_family = AF_UNIX;
	std::string neighbor_sock_path = NODE_BASE_SOCK_PATH + std::to_string((ring_id - 1) % ring_size);
	const char* neighbor_sock = neighbor_sock_path.c_str();
	strcpy(neighbor_addr.sun_path, neighbor_sock);
	return neighbor_addr;
}

void Node::sendElectionMessage(int messagePID)
{
	struct sockaddr_un neighbor_addr = leftNeighbor();

	RingMessage electionMessage;
	electionMessage.type = MessageType::Election;
	electionMessage.sourceRingID = ring_id;
	electionMessage.messagePID = messagePID;

	char buffer[256];
	memcpy(buffer, &electionMessage, sizeof(electionMessage));
	sendMessage(neighbor_addr, buffer, strlen(buffer));
}

void Node::sendLeaderMessage(int sourceRingID)
{
	struct sockaddr_un neighbor_addr = leftNeighbor();

    RingMessage leaderMessage;
    leaderMessage.type = MessageType::Leader;
    leaderMessage.sourceRingID = sourceRingID;
    leaderMessage.messagePID = -1;

    char buffer[256];
	memcpy(buffer, &leaderMessage, sizeof(leaderMessage));
    sendMessage(neighbor_addr, buffer, strlen(buffer));
}