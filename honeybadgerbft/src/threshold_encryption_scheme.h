#ifndef THRESHOLDENCRYPTIONSCHEME_H
#define THRESHOLDENCRYPTIONSCHEME_H

#include "tcg.h"
#include <vector>
#include <string>

struct ThresholdEncryptionScheme {
    std::vector<Transaction> encrypt(const std::vector<Transaction>& transactions);
    std::vector<Transaction> decrypt(const std::vector<Transaction>& encryptedTransactions);
};

#endif