#ifndef _SERVER_H
#define _SERVER_H

#include "configurator.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

struct BlockAttachment {
    uint64_t rank;
    uint64_t nextRank;
};

class OHIENode {
public:
    OHIENode(int id, int numChains)
        : nodeId(id), numChains(numChains) {}

    void run() {
        initializeGenesisBlocks();

        while (true) {
            receiveState();
            mineBlock();
            sendState();
        }
    }

private:
    int nodeId;
    int numChains;

    std::vector<std::vector<Block>> chains;
    std::vector<MerkleTree> merkleTrees;
    Block trailingBlock;

    void initializeGenesisBlocks();

    void receiveState();
    void mineBlock();
    void sendState();
};

#endif