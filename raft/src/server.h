#ifndef _SERVER_H
#define _SERVER_H

#include "configurator.h"
#include "Raft.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

class RaftServerImpl final: public Raft::Server {
public:
	explicit RaftServerImpl(const std::string& nodeId);
    kj::Promise<void> appendEntry(AppendEntryContext context) override;
    kj::Promise<void> requestVote(RequestVoteContext context) override;
    kj::Promise<void> clientRequest(ClientRequestContext context) override;

private:
	void setElectionTimer();
    void handleElectionTimeout();
    void convertToCandidate();
    void requestVotes();
    void convertToLeader();
    void leaderHeartbeat();

	NodeConfig nodeConfig;
    std::unordered_map<uuid_t, Raft::Client> peers;
    std::unordered_map<NodeConfig> clusterConfig;

	int currentTerm;
	std::string votedFor;
	vector<Entry>log;

	int commitIndex;
	int lastIndex;

	std::unordered_map<uuid_t, int> nextIndex;
    std::unordered_map<uuid_t, int> matchIndex;
    std::chrono::system_clock::time_point lastHeartbeatTime;

	enum class ServerState{
		FOLLOWER,
		CANDIDATE,
		LEADER
	} currentState;
}

#endif