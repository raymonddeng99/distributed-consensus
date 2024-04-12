#include "server.h"

PBFTServerImpl::PBFTServerImpl(const NodeConfig nodeConfig, std::unordered_map < NodeConfig > clusterConfig): NodeConfig(nodeConfig), clusterConfig(clusterConfig) {
  pbftState.prePrepareOngoing = false;
}

kj::Promise<void>PBFTServerImpl::clientRequest(ClientRequestContext context) {
  int command = context.getParams().getCommand();

  if (currentState == ServerState::LEADER) {
    pbftState.prePrepareOngoing = true;
    pbftState.requestDigest = generateDigest(command);
    broadcastPrePrepare(command, pbftState.requestDigest);
  }

  context.getResults().setLeaderId(identifier).setSuccess(true);
  return kj::READY_NOW;
}

void PBFTServerImpl::broadcastPrePrepare(int command,
  const std::string & requestDigest) {
  for (const auto & node: peers) {
    auto client = node.second;
    auto request = client.prePrepare();
    request.setViewNum(currentViewNumber);
    request.setSequenceNumber(nextIndex);
    request.setMessageDigest(requestDigest);
    request.then({});
  }
  nextIndex++;
}

kj::Promise <void> PBFTServerImpl::appendEntry(AppendEntryContext context) {
  int messageType = context.getParams().getMessageType();
  switch (messageType) {
  case PRE_PREPARE_MSG:
    handlePrePrepare(context);
    break;
  case PREPARE_MSG:
    handlePrepare(context);
    break;
  case COMMIT_MSG:
    handleCommit(context);
    break;
  default:
    break;
  }
  context.getResults().setTerm(currentTerm).setSuccess(true);
  return kj::READY_NOW;
}

void PBFTServerImpl::handlePrePrepare(AppendEntryContext context) {
  if (!pbftState.prePrepareOngoing || context.getParams().getMessageDigest() != pbftState.requestDigest) {
    return;
  }
  pbftState.prepareVotes[context.getParams().getReplicaId()] = true;
  if (checkMajorityVote(pbftState.prepareVotes)) {
    pbftState.prePrepareOngoing = false;
    broadcastCommit(context.getParams().getMessageDigest());
  }
}

void PBFTServerImpl::handlePrepare(AppendEntryContext context) {
}

void PBFTServerImpl::handleCommit(AppendEntryContext context) {
  if (!pbftState.prePrepareOngoing || context.getParams().getMessageDigest() != pbftState.requestDigest) {
    return;
  }
  if (currentState == ServerState::LEADER) {
  }
}

bool PBFTServerImpl::checkMajorityVote(const std::unordered_map < uuid_t, bool > & votes) {
  int totalVotes = 0;
  for (const auto & [_, vote]: votes) {
    if (vote) {
      totalVotes++;
    }
  }
  return totalVotes > clusterConfig.size() / 2;
}

void PBFTServerImpl::broadcastCommit(const std::string & requestDigest) {
  for (const auto & node: peers) {
    auto client = node.second;
    auto request = client.commit();
    request.setViewNum(currentViewNumber);
    request.setSequenceNumber(nextIndex - 1);
    request.setMessageDigest(requestDigest);
    request.then({});
  }
}

std::string PBFTServerImpl::generateDigest(int command) {
  std::stringstream ss;
  ss << command;
  return ss.str();
}