#include "server.h"

PaxosServerImpl::PaxosServerImpl(const NodeConfig nodeConfig, const ClusterConfig clusterConfig) : nodeConfig(nodeConfig), clusterConfig(clusterConfig){
	for (const auto& nodeConfig: clusterConfig){
	    if (nodeConfig == this->nodeConfig){
	      continue;
	    }

	    auto server = kj::heap<RaftServerImpl>(nodeConfig.ipAddress);
	    auto& serverRef = *server;
	    Raft::Client capability = kj::mv(server);
	    peers.insert({node.uuid, capability});   
  }
};

void PaxosServerImpl::sendPrepare(int proposalValue){
	int votes = 0;
	int quorum = std::ceil(this->peers.length / 2);

	for (const auto& node: this->peers){
		auto io = kj::setupAsyncIo();
		auto client = node->second;

		auto request = client.requestPrepare();
		request.setNumber(proposalIndex);
		request.setValue(proposalValue);

		request.then([](kj::RequestPrepareResult) {
			int responseProposalNumber = RequestPrepareResult.getProposalNumber();

			if (followerProposalIndex == proposalIndex){
				votes += 1;
			}
			if (votes >= quorum){
				sendAccept();
			}
		})
	}
}

kj::Promise<void> PaxosServerImpl::prepare(PrepareContext context){
	std::cout << "Prepare request..." << std::endl;
	std::cout.flush();

	int proposalNum = context.getParams().getNumber();

	if (proposalIndex > proposalNum){
		context.getResults().setProposalNum(proposalIndex);
	    return kj::READY_NOW;
	};
};

kj::Promise<void> PaxosServerImpl::accept(AcceptContext context){
	std::cout << "Accept request..." << std::endl;
	std::cout.flush();

	int proposalNumber = context.getParams().getNumber();
	int proposalValue = context.getParams().getValue();

	if (proposalNumber < acceptedProposalNumber){
		context.getResults().setProposalNum(acceptedProposalNumber);
	    return kj::READY_NOW;
	}

	acceptedProposalNumber = max(acceptedProposalNumber, proposalNumber);
}