#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <vector>
#include <string>

const int HASH_SIZE = 20; // SHA-1 hash size
const int CIPHERTEXT_SIZE = 2 * EC_POINT_SIZE + HASH_SIZE;
const int SHARE_SIZE = EC_POINT_SIZE + sizeof(int);

BIGNUM* hash_to_bn(const std::string& input);
EC_POINT* bn_to_point(const BIGNUM* bn, EC_GROUP* group);
std::string point_to_str(const EC_POINT* point, EC_GROUP* group);

struct PublicKey {
    EC_POINT* Y;
};

struct PrivateKey {
    int id;
    BIGNUM* x;
};

struct Ciphertext {
    EC_POINT* U;
    std::string V;
    EC_POINT* W;
};

struct Share {
    int id;
    EC_POINT* U;
};

std::vector<PrivateKey> generate_private_keys(int n, int t, EC_GROUP* group, PublicKey& pub_key);

Ciphertext encrypt(const std::string& message, const PublicKey& pub_key, EC_GROUP* group);

Share generate_share(const Ciphertext& ciphertext, const PrivateKey& priv_key, EC_GROUP* group);

bool verify_share(const Ciphertext& ciphertext, const Share& share, const PublicKey& pub_key, EC_GROUP* group);

std::string decrypt(const Ciphertext& ciphertext, const std::vector<Share>& shares, const PublicKey& pub_key, EC_GROUP* group);