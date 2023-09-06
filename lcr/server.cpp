#include "server.h"

Server::Server(const char* _SOCK_PATH) : SOCK_PATH(_SOCK_PATH) {
	// if (signal(SIGINT, sigintHandler) == SIG_ERR) {
	// 	fprintf(stderr, "Failed to register the signal handler.\n");
	// 	return 1;
	// }

	server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1)
    {
        printf("Opening socket %s failed!\n", SOCK_PATH);
        exit(1);
    }

    struct sockaddr_un node_addr;
    memset(&node_addr, 0, sizeof(node_addr));
    node_addr.sun_family = AF_UNIX;
    strcpy(node_addr.sun_path, SOCK_PATH);
    unlink(SOCK_PATH);

    int rc = bind(server_sock, (struct sockaddr *)&node_addr, sizeof(node_addr));
    if (rc == -1){
        printf("SERVER: Node bind error: %s\n", strerror(errno));
        close(server_sock);
        exit(1);
    }

    rc = listen(server_sock, 10);
    if (rc == -1){
        printf("SERVER: Listen error: %s\n", strerror(errno));
        close(server_sock);
        exit(1);
    }

    printf("SERVER %s listening...\n", SOCK_PATH);
}

void Server::start()
{
	while(true)
	{
		receiveMessage();
	}
}

void Server::sendMessage(struct sockaddr_un target_addr, char* messageBuf, size_t size){
	int client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	int rc = connect(client_sock, (struct sockaddr*)&target_addr, sizeof(target_addr));
    if(rc == -1)
    {
        printf("SERVER: Connect error. %s\n", strerror(errno));
        close(client_sock);
        exit(1);
    }

    rc = send(client_sock, messageBuf, size, 0);
    if (rc == -1)
    {
        printf("NODE: Send error. %s\n", strerror(errno));
        close(client_sock);
        exit(1);
    }

    close(client_sock);
}

void Server::receiveMessage(){
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientSocket = accept(server_sock, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        perror("Accepting connection failed");
        exit(1);
    }

    // std::thread clientThread(handleClient, clientSocket);
    std::thread clientThread([this, clientSocket]() {
        this->handleClient(clientSocket);
    });
    clientThread.detach();
}

void Server::handleClient(int clientSocket) 
{
    while (true) 
    {
    	char message_buffer[256];
    	memset(message_buffer, 0, sizeof(message_buffer));

        int bytesRead = recv(clientSocket, message_buffer, sizeof(message_buffer), 0);
        if (bytesRead <= 0) {
            close(clientSocket);
            return;
        }

        messageHandler(message_buffer, sizeof(message_buffer));

        close(clientSocket);
    }
}

void Server::messageHandler(char* message_buffer, size_t size_message_buffer) {};

void Server::sigintHandler(int signal)
{
	terminate();
}

void Server::terminate()
{
	if (remove(SOCK_PATH) != 0) 
	{
		perror("Error removing socket file");
	}
    if (close(server_sock) != 0)
    {
        perror("Error closing socket");
    }
}