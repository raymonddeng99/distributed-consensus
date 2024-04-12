#ifndef STREAMLETSERVER_H
#define STREAMLETSERVER_H

#include "configurator.h"
#include "streamlet.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

class StreamletServer : public StreamletNode::Server {
public:
    kj::Promise<void> propose(ProposeContext context) override;
    kj::Promise<void> vote(VoteContext context) override;
};

#endif