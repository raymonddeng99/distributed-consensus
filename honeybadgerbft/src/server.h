#ifndef HONEYBADGERNODE_H
#define HONEYBADGERNODE_H

#include "threshold_encryption_scheme.h"
#include "configurator.h"
#include "PBFT.capnp.h"
#include <capnp/rpc.h>
#include <capnp/serialize.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <set>

struct Transaction {
    std::string id;
    std::string payload;
};

class HoneyBadgerNode {
public:
    HoneyBadgerNode(int id, const ThresholdEncryptionScheme& encryptionScheme);

    void run();

private:
    int nodeId;
    ThresholdEncryptionScheme thresholdEncryptionScheme;
    std::queue<Transaction> transactionBuffer;
    std::vector<std::vector<Transaction>> committedBatches;

    std::vector<Transaction> selectRandomTransactionsFromBuffer();

    void commitBatch(const std::vector<Transaction>& batch);
};

#endif