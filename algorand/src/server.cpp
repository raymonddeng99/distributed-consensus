#include "server.h"

RaftServerImpl::RaftServerImpl(const NodeConfig nodeConfig, std::unordered_map<NodeConfig> clusterConfig) : NodeConfig(nodeConfig), clusterConfig(clusterConfig) {
  currentTerm = 0;
  votedFor = "";
  log.clear();
  commitIndex = 0;
  lastIndex = 0;
  nextIndex = 0;
  matchIndex = 0;

  for (const auto& nodeConfig: clusterConfig){
    if (nodeConfig == this->nodeConfig){
      continue;
    }

    auto server = kj::heap<RaftServerImpl>(nodeConfig.ipAddress);
    auto& serverRef = *server;
    Raft::Client capability = kj::mv(server);
    peers.insert({node.uuid, capability});   
  }

  currentState = ServerState::FOLLOWER;
  setElectionTimer();
};

void RaftServerImpl::setElectionTimer() {
    int minTimeout = 150;
    int maxTimeout = 300;
    int randomTimeout = rand() % (maxTimeout - minTimeout + 1) + minTimeout;
    std::chrono::milliseconds timeout(randomTimeout);
    kj::setTimer(timeout, [this]() {
        handleElectionTimeout();
    });
}

void RaftServerImpl::handleElectionTimeout() {
  auto currentTime = std::chrono::system_clock::now();
  bool communicationTimeout = (curentTime - lastHeartbeatTime >= electionTimeout);
  if (communicationTimeout){
    convertToCandidate();
    setElectionTimer();
  }
};

void RaftServerImpl::convertToCandidate() {
    currentTerm++;
    currentState = ServerState::CANDIDATE;
    votedFor = identifier;
    requestVotes();
};

void RaftServerImpl::requestVotes(){
  int votes = 0;
  int quorum = std::ceil(this->peers.length / 2);

  for (const auto& node: this->peers){
    auto io = kj::setupAsyncIo();
    auto client = node->second;

    auto request = client.requestVote();
    request.setTerm(currentTerm);
    request.setCandidateId(nodeConfig.uuid);
    request.setLastLogIndex(lastIndex);
    request.setLastLogTerm(log[lastIndex].term);

    request.then([](kj::RequestVoteResult){
      bool voteGranted = RequestVoteResult.getVoteGranted();
      if (voteGranted){
        votes += 1;
      }
      if (votes >= quorum){
        convertToLeader();
      }
    })
  };
}

void RaftServerImpl::convertToLeader(){
  if (this->currentState == ServerState::LEADER){
    return;
  }
  currentState = ServerState::LEADER;
  leaderHeartbeat();    // repeat during idle periods to prevent election timeouts
}

void RaftServerImpl::leaderHeartbeat(){
  for (const auto& node: this->peers){
    auto io = kj::setupAsyncIo();
    auto client = node->second;

    auto request = client.appendEntry();
    request.setTerm(currentTerm);
    request.setLeaderId(nodeConfig.uuid);
    request.setPrevLogInex(lastIndex);
    request.setLastLogTerm(log[lastIndex].term);
    request.setEntries([]);
    request.setLeaderCommitIndex(commitIndex);

    request.then([](kj::AppendEntryResult){
      // 
    })
  };
}


kj::Promise<void> RaftServerImpl::requestVote(RequestVoteContext context){
  std::cout << "Received request vote...";
  std::cout.flush();

  auto io = kj::setupAsyncIo();

  int term = context.getParams().getTerm();
  if (term < currentTerm){
    context.getResults().setTerm(currentTerm).setVoteGranted(false);
    return kj::READY_NOW;
  };

  int candidateId = context.getParams().getCandidateId();
  int lastLogIndex = context.getParams().getLastLogIndex();

  bool canVoteForCandidate = (votedFor == "" || votedFor == candidateId);
  bool validCandidate = (lastLogIndex >= lastIndex)
  if (canVoteForCandidate && validCandidate){
    context.getResults().setTerm(term).setVoteGranted(true);
    return kj::READY_NOW;
  }
}

kj::Promise<void> appendEntry(AppendEntryContext context){
  std::cout << "Received append entry...";
  std::cout.flush();

  auto io = kj::setupAsyncIo();

  // if appendEntry RPC received from new leader, convert to follower

  int term = context.getParams().getTerm();
  if (term < currentTerm){
    context.getResults().setTerm(currentTerm).setSuccess(false);
    return kj::READY_NOW;
  };

  int prevLogTerm = context.getParams().getPrevLogTerm();
  int prevLogIndex = context.getParams().getPrevLogIndex();
  if (log[prevLogIndex].term != prevLogTerm){
    context.getResults().setTerm(currentTerm).setSuccess(false);
    return kj::READY_NOW;
  }

  // if an existing entry conflicts with a new one, same index but different terms,
  // delete that existing entry and all that follow it

  auto entries = context.getParams().getEntries();
  for (int i = 0; i < entries.length; i++){
    log.emplace_back(entries[i]);
  }

  int leaderCommit = context.getParams().getLeaderCommit();
  if (leaderCommit > commitIndex){
    int latestEntry = entries[entries[entries.length - 1]];
    commitIndex = min(leaderCommit, latestEntry.logIndex);
  }

  context.getResults().setTerm(currentTerm).setSuccess(true);
  return kj::READY_NOW;
}


kj::Promise<void> RaftServerImpl::clientRequest(ClientRequestContext context){
  std::cout << "Servicing client request...";
  std::cout.flush();

  auto io = kj::setupAsyncIo();

  int command = context.getParams().getCommand();
  log.emplace_back(command);

  for (const auto& node: this->peers){
    auto io = kj::setupAsyncIo();
    auto client = node->second;

    // if last log index >= nextIndex for a follower,
    // send appendEntries RPC with log entries starting at nextIndex

    auto request = client.appendEntry();
    request.setTerm(currentTerm);
    request.setLeaderId(nodeConfig.uuid);
    request.setPrevLogInex(lastIndex);
    request.setLastLogTerm(log[lastIndex].term);
    request.setEntries([command]);
    request.setLeaderCommitIndex(commitIndex);
    request.then([](kj::AppendEntryResult){
      bool success = AppendEntryResult.getSuccess();
      if (success){
        this->nextIndex[node.uuid] += 1;
        this->matchIndex[node.uuid] += 1;
      }
      else{
        fixLogInconsistency(node, nextIndex - 1);
      }
    })
  };

  // if there's N s.t.:
  //    N > commitIndex
  //    majority of matchIndex[i] >= N
  //    log[N].term == currentTerm

  // commitIndex := N

  context.getResults().setLeaderId(identifier).setSuccess(true);
  return kj::READY_NOW;
}

void RaftServerImpl::fixLogInconsistency(NodeConfig inconsistentNode, int nextIndex){
  auto io = kj::setupAsyncIo();

  auto client = peers[inconsistentNode.identifier];

  auto request = client.appendEntry();
  request.setTerm(currentTerm);
  request.setLeaderId(this->nodeConfig.uuid);
  request.setPrevLogIndex(lastIndex);
  request.setLastLogTerm(log[lastIndex].term);
  request.setEntries([command]);
  request.setLeaderCommitIndex(commitIndex);
  request.wait(io.waitScope);

  bool success = request.getSuccess();
  if (!success){
    fixLogInconsistency(inconsistentNode, nextIndex - 1);
  }
}