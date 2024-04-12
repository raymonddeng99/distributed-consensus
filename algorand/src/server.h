#ifndef _SERVER_H
#define _SERVER_H

#include "configurator.h"
#include "bastar.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

class BAStarServer : public BAStarNode::Server {
public:
    kj::Promise<void> sendVote(SendVoteContext context) override {
        auto msg = context.getParams().getMessage();
        auto type = msg.getType();
        auto round = msg.getRound();
        auto step = msg.getStep();
        auto value = msg.getValue();
        auto sorthash = msg.getSorthash();
        auto sortproof = msg.getSortproof();
        auto signature = msg.getSignature();

        return kj::READY_NOW;
    }

    kj::Promise<void> sendConfirm(SendConfirmContext context) override {
        auto msg = context.getParams().getMessage();
        auto type = msg.getType();
        auto round = msg.getRound();
        auto step = msg.getStep();
        auto value = msg.getValue();
        auto sorthash = msg.getSorthash();
        auto sortproof = msg.getSortproof();
        auto signature = msg.getSignature();

        return kj::READY_NOW;
    }
};

#endif