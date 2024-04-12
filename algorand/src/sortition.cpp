#include <cstdint>
#include <string>
#include <utility>
#include "crypto/vrf.h"

static const int HASH_LEN = 32;

std::pair<std::string, std::string> sortition(const std::string& secretKey, uint64_t seed, uint8_t role, uint64_t w, uint64_t W) {
    std::string seedRole = std::to_string(seed) + "|" + std::to_string(role);
    auto [hash, proof] = generateVRF(secretKey, seedRole);

    uint64_t p = static_cast<uint64_t>(w) * UINT64_MAX / W;
    uint64_t j = 0;
    uint64_t hashNum = 0;
    memcpy(&hashNum, hash.data(), std::min(hash.size(), sizeof(hashNum)));

    uint64_t sum = 0;
    uint64_t binomialVal;
    do {
        binomialVal = binomialCoeff(w, j) * pow(p, j) * pow(1 - p, w - j);
        sum += binomialVal;
        j++;
    } while (hashNum >= sum * UINT64_MAX);
    j--;

    return {hash, proof, std::to_string(j)};
}

bool verifySort(const std::string& publicKey, const std::string& hash, const std::string& proof, uint64_t seed, uint8_t role, uint64_t w, uint64_t W) {
    std::string seedRole = std::to_string(seed) + "|" + std::to_string(role);
    if (!verifyVRF_C(publicKey, hash, proof, seedRole)) {
        return false;
    }

    uint64_t p = static_cast<uint64_t>(w) * UINT64_MAX / W;
    uint64_t j = 0;
    uint64_t hashNum = 0;
    memcpy(&hashNum, hash.data(), std::min(hash.size(), sizeof(hashNum)));

    uint64_t sum = 0;
    uint64_t binomialVal;
    do {
        binomialVal = binomialCoeff(w, j) * pow(p, j) * pow(1 - p, w - j);
        sum += binomialVal;
        j++;
    } while (hashNum >= sum * UINT64_MAX);
    j--;

    return j == std::stoull(proof);
}