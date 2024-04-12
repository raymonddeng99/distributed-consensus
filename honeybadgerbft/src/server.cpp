#include "server.h"

HoneyBadgerNode::HoneyBadgerNode(int id, const ThresholdEncryptionScheme& encryptionScheme)
    : nodeId(id), thresholdEncryptionScheme(encryptionScheme) {}

void HoneyBadgerNode::run() {
    while (true) {
        std::vector<Transaction> proposedTransactions = selectRandomTransactionsFromBuffer();
        std::vector<Transaction> encryptedProposals = thresholdEncryptionScheme.encrypt(proposedTransactions);

        std::vector<std::vector<Transaction>> acsOutput;

        std::vector<Transaction> newBatch;
        for (const auto& encryptedProposal : acsOutput) {
            std::vector<Transaction> decryptedProposal = thresholdEncryptionScheme.decrypt(encryptedProposal);
            newBatch.insert(newBatch.end(), decryptedProposal.begin(), decryptedProposal.end());
        }
        std::sort(newBatch.begin(), newBatch.end(), [](const Transaction& a, const Transaction& b) {
            return a.id < b.id;
        });

        commitBatch(newBatch);
    }
}

std::vector<Transaction> HoneyBadgerNode::selectRandomTransactionsFromBuffer() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<Transaction> proposedTransactions;
    int batchSize = transactionBuffer.size() / numNodes;

    std::sample(transactionBuffer.begin(), transactionBuffer.end(),
                std::back_inserter(proposedTransactions), batchSize, gen);

    return proposedTransactions;
}

void HoneyBadgerNode::commitBatch(const std::vector<Transaction>& batch) {
    committedBatches.push_back(batch);

    std::set<Transaction> batchSet(batch.begin(), batch.end());
    std::queue<Transaction> newBuffer;
    std::copy_if(transactionBuffer.begin(), transactionBuffer.end(), std::back_inserter(newBuffer),
                 [&](const Transaction& tx) { return batchSet.count(tx) == 0; });
    transactionBuffer = std::move(newBuffer);
}
