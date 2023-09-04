#include "constants.h"
#include "coordinator.h"

int main() {
    int numProcesses;
    std::cout << "Enter the number of processes in the ring: ";
    std::cin >> numProcesses;

    Coordinator coordinator(COORDINATOR_SOCK_PATH, numProcesses);

    return 0;
}