#include "StreamletServer.h"

kj::Promise<void> StreamletServer::propose(ProposeContext context) {
    auto msg = context.getParams().getMsg();
    auto type = msg.getType();
    auto epoch = msg.getEpoch();
    auto payload = msg.getPayload();
    auto signature = msg.getSignature();

    return kj::READY_NOW;
}

kj::Promise<void> StreamletServer::vote(VoteContext context) {
    auto msg = context.getParams().getMsg();
    auto type = msg.getType();
    auto epoch = msg.getEpoch();
    auto payload = msg.getPayload();
    auto signature = msg.getSignature();

    return kj::READY_NOW;
}