#ifndef _SERVER_H
#define _SERVER_H

class RaftServerImpl final: public RaftProtocol::Server {
public:
	explicit RaftServerImpl(const std::string& nodeId);
    kj::Promise<NodeInfo::Reader> appendEntry(FindSuccessorContext context) override;
    kj::Promise<NodeInfo::Reader> requestVote(FindPredecessorContext context) override;

    kj::Promise<NodeInfo::Reader> handleAppendEntry(HandleAppendEntryContext context) override;
    kj::Promise<NodeInfo::Reader> handleRequestVote(HandleRequestVoteContext context) override;

private:
	int currentTerm;
	std::string votedFor;
	vector<std::string> log;

	int commitIndex;
	int lastIndex;

	int nextIndex;
	int matchIndex;

	enum class ServerState{
		FOLLOWER,
		CANDIDATE,
		LEADER
	}
	ServerState currentState;
}

#endif