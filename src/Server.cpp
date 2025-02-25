#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"

Server::Server(int port, const char* password)
{
	this->port = port;
	this->password = password;
}

int Server::getSock() const
{
	return serverSocket;
}

void	Server::run()
{
	int opt = 1;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
	   perror("socket");
		exit(1);
	}
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,  (char *) &opt, sizeof(opt)))
	{
		perror("setsockopt");
		close(serverSocket);
		exit(1);
	}
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) 
	{
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
	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		perror("listen");
		close(serverSocket);
		exit(1);
	}
	std::cout << "Server is listening on port " << port << '\n';
}

void Server::connect_client(Server &server)
{
	struct pollfd new_poll;
	new_poll.fd = server.getSock();
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	fds.push_back(new_poll);

	while (true)
	{
		int ready = poll(&fds[0], fds.size(), 0);
		if (ready == -1)
		{
			perror("poll");
			break;
		}

		// Check if there is activity on the listening socket
		if (fds[0].revents & POLLIN)
		{
			struct sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			int clientSocket = accept(server.getSock(), (struct sockaddr *)&clientAddress, &clientAddressLength);

			if (clientSocket == -1)
			{
				perror("accept");
				continue;  // Retry accepting new connections
			}

			struct pollfd new_client;
			new_client.fd = clientSocket;
			new_client.events = POLLIN;
			new_client.revents = 0;

			Client newClient;
			newClient.setSocket(clientSocket);
			newClient.setHost(inet_ntoa(clientAddress.sin_addr));

			// Insert the client into the map with key = clientSocket
			clients[clientSocket] = newClient;

			fds.push_back(new_client);
			std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << '\n';
		}

		// Check all connected clients for activity
		for (size_t i = 1; i < fds.size(); i++)  // Start at 1 to avoid the server socket itself
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[1024];
				ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (bytesRead == -1)
				{
					perror("read");
					continue;
				}

				// Client disconnect
				if (bytesRead == 0) {
					std::cout << "Client " << fds[i].fd << " disconnected." << '\n';
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					i--;
					continue;
				}

				buffer[bytesRead] = '\0';
				std::string message(buffer);
				std::cout << "Client " << fds[i].fd << " sent: " << message << '\n';
				//handle authentification and add the client to the server container
				// Use map lookup instead of looping over a vector
				// if (clients.find(fds[i].fd) != clients.end())
				// {
					//if the client is authentificate
					// if (client.getIs_auth() == true)
				parse_message(message, clients[fds[i].fd], clients, channels, password);
				//}
			}
		}
	}
	// Cleanup: Close all file descriptors before program termination
	for (size_t i = 0; i < fds.size(); i++) {
		close(fds[i].fd);
	}
}

