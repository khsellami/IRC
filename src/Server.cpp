
#include "../include/header.hpp"

Server::Server(){}

Server::Server(int port, std::string password)
{
	this->port = port;
	this->password = password;
}
Channel* Server::getChannel(const std::string& channelName) {
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++) {
        if (it->first == channelName)
			return &it->second;
    }
    return NULL;
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
	try {
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

		if (fds[0].revents & POLLIN)
		{
			struct sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			int clientSocket = accept(server.getSock(), (struct sockaddr *)&clientAddress, &clientAddressLength);

			if (clientSocket == -1)
			{
				perror("accept");
				continue;
			}

			struct pollfd new_client;
			new_client.fd = clientSocket;
			new_client.events = POLLIN;
			new_client.revents = 0;

			Client newClient;
			newClient.setSocket(clientSocket);
			newClient.setHost(inet_ntoa(clientAddress.sin_addr));

			clients[clientSocket] = newClient;

			fds.push_back(new_client);
			std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << '\n';
		}

		for (size_t i = 1; i < fds.size(); i++)
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
				parse_message(message, clients[fds[i].fd], server);
			}
		}
	}
	}
	catch(const char *e)
	{
	for (size_t i = 0; i < fds.size(); i++)
	{
		close(fds[i].fd);
	}
	exit(1);
	}
}

std::map<std::string, Channel>& Server::getChannels() {
    return channels;
}



void Server::addChannel(std::string name, Channel channel)
{
	channels[name] = channel;
}

Client* Server::getClientByName(const std::string &name)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickName() == name)
			return &clients[i];
	}
	return NULL;
}
std::string Server::getPassword(){
	return password;
}
std::map<int , Client>& Server::getClients(){return clients;}