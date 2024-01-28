#include "server.h"
#include <capnp/ez-rpc.h>
#include "RaftProtocol.capnp.h"
#include <capnp/serialize.h>
#include <iostream>

explicit RaftServerImpl(const std::string& nodeId) : identifier(nodeId) {
	currentTerm = 0;
    votedFor = "";
    log.clear();
    commitIndex = 0;
    lastIndex = 0;
    nextIndex = 0;
    matchIndex = 0;

    currentState = ServerState::FOLLOWER;
    resetElectionTimeout();
};

kj::Promise<NodeInfo::Reader> appendEntry(FindSuccessorContext context){
    return; 
}

kj::Promise<NodeInfo::Reader> requestVote(FindPredecessorContext context){
    return;
}

kj::Promise<NodeInfo::Reader> handleAppendEntry(HandleAppendEntryContext context){
	// Get the parameters from the context
  int term = context.getParams().getTerm();
  int prevLogIndex = context.getParams().getPrevLogIndex();
  int prevLogTerm = context.getParams().getPrevLogTerm();
  const LogEntry::Reader entries = context.getParams().getEntries();
  int leaderCommit = context.getParams().getLeaderCommit();

  // Create a response promise and resolver
  kj::PromiseFulfiller<NodeInfo::Reader> fulfiller = context.getResults();

  // Check if the received term is less than the current term
  if (term < currentTerm) {
    // Reply false if the term is less than the current term
    fulfiller.reject(kj::Exception("Stale term"));
    return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
  }

  // Check if the log contains the entry at prevLogIndex with the matching term
  if (prevLogIndex >= log.size() || log[prevLogIndex].term() != prevLogTerm) {
    // Reply false if the log doesn't contain the entry at prevLogIndex with the matching term
    fulfiller.reject(kj::Exception("Log inconsistency"));
    return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
  }

  // Process the log entries
  int currentIndex = prevLogIndex + 1;
  int entriesIndex = 0;
  while (entriesIndex < entries.size() &&
         currentIndex < log.size() &&
         log[currentIndex].term() == entries[entriesIndex].term()) {
    // Check if the log entry conflicts with the new one
    if (log[currentIndex].term() == entries[entriesIndex].term()) {
      // Delete existing entry and all that follow
      log.erase(log.begin() + currentIndex, log.end());
      break;
    }
    currentIndex++;
    entriesIndex++;
  }

  // Append any new entries not already in the log
  while (entriesIndex < entries.size()) {
    log.push_back(entries[entriesIndex]);
    entriesIndex++;
  }

  // Update the commit index
  commitIndex = std::min(leaderCommit, static_cast<int>(log.size()) - 1);

  // Respond with the current term and success
  NodeInfo::Builder responseBuilder = context.getResults().getResults();
  responseBuilder.setTerm(currentTerm);
  fulfiller.fulfill(responseBuilder.asReader());
  return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
}

kj::Promise<NodeInfo::Reader> handleRequestVote(HandleRequestVoteContext context){
	// Get the parameters from the context
  int term = context.getParams().getTerm();
  int candidateId = context.getParams().getCandidateId();
  int lastLogIndex = context.getParams().getLastLogIndex();
  int lastLogTerm = context.getParams().getLastLogTerm();

  // Create a response promise and resolver
  kj::PromiseFulfiller<NodeInfo::Reader> fulfiller = context.getResults();

  // Reply false if term < currentTerm
  if (term < currentTerm) {
    fulfiller.reject(kj::Exception("Stale term"));
    return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
  }

  // If votedFor is null or candidateId, and candidate’s log is at least as up-to-date as receiver’s log, grant vote
  if ((votedFor.empty() || votedFor == std::to_string(candidateId)) &&
      (lastLogTerm > currentTerm || (lastLogTerm == currentTerm && lastLogIndex >= lastIndex))) {
    // Grant the vote
    votedFor = std::to_string(candidateId);

    // Respond with the current term and voteGranted true
    NodeInfo::Builder responseBuilder = context.getResults().getResults();
    responseBuilder.setTerm(currentTerm);
    responseBuilder.setVoteGranted(true);
    fulfiller.fulfill(responseBuilder.asReader());
    return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
  }

  // Reply false otherwise
  fulfiller.reject(kj::Exception("Vote not granted"));
  return kj::Promise<NodeInfo::Reader>(fulfiller.getPromise());
}


void RaftServerImpl::resetElectionTimeout() {
    int minTimeout = 150;
    int maxTimeout = 300;
    int randomTimeout = rand() % (maxTimeout - minTimeout + 1) + minTimeout;

    std::chrono::milliseconds timeout(randomTimeout);

    kj::setTimer(timeout, [this]() {
        handleElectionTimeout();
    });
}

void RaftServerImpl::handleElectionTimeout() {
}
