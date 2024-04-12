#include <string>
#include <uuid/uuid.h>

struct NodeConfig {
	uuid_t uuid;
	std::string ipAddress;
};