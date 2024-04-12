#include "PrismServer.h"

kj::Promise<void> PrismServer::propose(ProposeContext context) {
    auto msg = context.getParams().getMsg();
    auto type = msg.getType();
    auto epoch = msg.getEpoch();
    auto proposalId = msg.getProposalId();
    auto payload = msg.getPayload();
    auto signature = msg.getSignature();

    return kj::READY_NOW;
}

kj::Promise<void> PrismServer::vote(VoteContext context) {
    auto msg = context.getParams().getMsg();
    auto type = msg.getType();
    auto epoch = msg.getEpoch();
    auto proposalId = msg.getProposalId();
    auto payload = msg.getPayload();
    auto signature = msg.getSignature();

    return kj::READY_NOW;
}

kj::Promise<void> PrismServer::newEpoch(NewEpochContext context) {
    auto msg = context.getParams().getMsg();
    auto type = msg.getType();
    auto epoch = msg.getEpoch();
    auto proposalId = msg.getProposalId();
    auto payload = msg.getPayload();
    auto signature = msg.getSignature();

    return kj::READY_NOW;
}