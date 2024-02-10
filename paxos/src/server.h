#ifndef _SERVER_H
#define _SERVER_H

#include "configurator.h"
#include "Paxos.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

class PaxosServerImpl final: public Paxos::Server {
public:
	explicit PaxosServerImpl(const NodeConfig nodeConfig);
    kj::Promise<void> prepare(PrepareContext context) override;
    kj::Promise<void> accept(AcceptContext context) override;

private:
	NodeConfig nodeConfig;
    std::unordered_map<uuid_t, Paxos::Client> peers;
    std::unordered_map<NodeConfig> clusterConfig;

    int proposalNumber;
    int highestPrepareNumber;
};

#endif