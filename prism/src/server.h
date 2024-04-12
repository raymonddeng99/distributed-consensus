#include "configurator.h"
#include "prism.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

#ifndef PRISMSERVER_H
#define PRISMSERVER_H

class PrismServer : public PrismNode::Server {
public:
    kj::Promise<void> propose(ProposeContext context) override;
    kj::Promise<void> vote(VoteContext context) override;
    kj::Promise<void> newEpoch(NewEpochContext context) override;
};

#endif