#ifndef _SERVER_H
#define _SERVER_H

#include "configurator.h"
#include "PBFT.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

class PBFTServerImpl final: public PBFT::Server{
public:
	explicit PBFTServerImpl(const std::string nodeId);
    kj::Promise<void> appendEntry(AppendEntryContext context) override;
    kj::Promise<void> requestVote(RequestVoteContext context) override;
    kj::Promise<void> clientRequest(ClientRequestContext context) override;
private:
	NodeConfig nodeConfig;
    std::unordered_map<uuid_t, PBFT::Client> peers;
    std::unordered_map<NodeConfig> clusterConfig;

    int viewNum;
    std::vector<string> acceptedMessages;
    bool prePrepareOngoing;
    std::string requestDigest;
    std::unordered_map < uuid_t, bool > prepareVotes

	enum class ServerState{
		PRIMARY,
		BACKUP
	} currentState;
}

#endif