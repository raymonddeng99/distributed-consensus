#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

int main(){
	std::vector<NodeConfig> clusterConfig = {
        {"a1b2c3d4-e5f6-4a5b-9c8d-1a2b3c4d5e6f", "192.168.0.1"},
        {"b2c3d4e5-f6g7-5b6c-8d9e-2f3g4h5i6j7", "192.168.0.2"},
        {"c3d4e5f6-g7h8-6c7d-9e0f-3g4h5i6j7k8", "192.168.0.3"},
        {"d4e5f6g7-h8i9-7d8e-0f1g-4h5i6j7k8l9", "192.168.0.4"},
        {"e5f6g7h8-i9j0-8e9f-1g2h-5i6j7k8l9m0", "192.168.0.5"}
    };

    std::unordered_map<std::string, Algorand::Client> cluster;
    for (const auto& nodeConfig: clusterConfig){
    	auto server = kj::heap<AlgorandServerImpl>(nodeConfig.ipAddress);
    	auto& serverRef = *server;
	    Algorand::Client capability = kj::mv(server);
	    cluster.insert({node.uuid, capability});
    };

    return 0;
}