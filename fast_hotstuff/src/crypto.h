#include <iostream>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/bn.h>

// Function to generate a secp256k1 key pair
EC_KEY* generate_key_pair() {
    EC_KEY *key_pair = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!key_pair) {
        std::cerr << "Error creating EC key pair." << std::endl;
        return nullptr;
    }

    if (!EC_KEY_generate_key(key_pair)) {
        std::cerr << "Error generating EC key pair." << std::endl;
        EC_KEY_free(key_pair);
        return nullptr;
    }

    return key_pair;
}

// Function to sign a message using a private key
std::string sign_message(const std::string& message, const EC_KEY* private_key) {
    const unsigned char *msg = reinterpret_cast<const unsigned char*>(message.c_str());
    size_t msg_len = message.length();

    ECDSA_SIG *signature = ECDSA_do_sign(msg, msg_len, const_cast<EC_KEY*>(private_key));
    if (!signature) {
        std::cerr << "Error signing the message." << std::endl;
        return "";
    }

    // Convert the signature to a DER-encoded string
    unsigned char *der_signature = nullptr;
    int der_len = i2d_ECDSA_SIG(signature, &der_signature);
    if (der_len <= 0) {
        std::cerr << "Error converting signature to DER format." << std::endl;
        ECDSA_SIG_free(signature);
        return "";
    }

    std::string result(reinterpret_cast<char*>(der_signature), der_len);

    ECDSA_SIG_free(signature);
    OPENSSL_free(der_signature);

    return result;
}

// Function to verify a signature using a public key
bool verify_signature(const std::string& message, const std::string& signature, const EC_KEY* public_key) {
    const unsigned char *msg = reinterpret_cast<const unsigned char*>(message.c_str());
    size_t msg_len = message.length();

    const unsigned char *der_signature = reinterpret_cast<const unsigned char*>(signature.c_str());
    ECDSA_SIG *ecdsa_sig = d2i_ECDSA_SIG(nullptr, &der_signature, signature.length());
    if (!ecdsa_sig) {
        std::cerr << "Error converting signature from DER format." << std::endl;
        return false;
    }

    int result = ECDSA_do_verify(msg, msg_len, ecdsa_sig, const_cast<EC_KEY*>(public_key));

    ECDSA_SIG_free(ecdsa_sig);

    return (result == 1);
}