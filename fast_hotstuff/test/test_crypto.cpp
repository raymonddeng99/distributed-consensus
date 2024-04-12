#include "secp256k1.h";

int main() {
    // Generate key pair
    EC_KEY *key_pair = generate_key_pair();
    if (!key_pair) {
        std::cerr << "Error generating key pair." << std::endl;
        return 1;
    }

    // Example message to sign
    std::string message = "Hello, secp256k1!";

    // Sign the message
    std::string signature = sign_message(message, key_pair);
    if (signature.empty()) {
        std::cerr << "Error signing the message." << std::endl;
        EC_KEY_free(key_pair);
        return 1;
    }

    std::cout << "Signature: " << signature << std::endl;

    // Verify the signature
    if (verify_signature(message, signature, key_pair)) {
        std::cout << "Signature is valid." << std::endl;
    } else {
        std::cerr << "Signature is invalid." << std::endl;
    }

    // Clean up
    EC_KEY_free(key_pair);

    return 0;
}