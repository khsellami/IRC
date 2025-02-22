#include "../include/Server.hpp"

void	Server::run()
{
	int opt = 1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
       perror("socket");
        exit(1);
    }
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,  (char *) &opt, sizeof(opt))){
		perror("setsockopt");
		close(serverSocket);
		exit(1);
	}
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl");
		close(serverSocket);
		exit(1);
	}
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		perror("bind");
		close(serverSocket);
		exit(1);
    }
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
    	perror("listen");
    	close(serverSocket);
    	exit(1);
    }
    std::cout << "Server is listening on port " << port << std::endl;
}

