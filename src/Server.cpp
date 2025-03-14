
#include "../include/header.hpp"


bool	Server::signal_received_flag = false;
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal_received_flag = true;
}

void	Server::close_allfds()
{
	for (size_t i = 0; i < clients.size(); i++)
		close(clients[i].getSocket());
	if (serverSocket != -1)
		close(serverSocket);
}
Server::Server(){}

Server::Server(int port, std::string password)
{
	this->port = port;
	this->password = password;
}

Channel* Server::getChannel(const std::string& channelName)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
	{
        if (it->first == channelName)
			return &it->second;
    }
    return (NULL);
}

int Server::getSock() const
{
	return serverSocket;
}

std::map<std::string, Channel>& Server::getChannels()
{
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

std::string Server::getPassword()
{
	return password;
}

std::map<int , Client>& Server::getClients()
{
	return clients;
}

void	Server::run()
{
	int opt = 1;
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw "Error when creating socket";
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
		throw "Error when set socket in reuse address";
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw "Error when set socket in non bloquant";
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw "Error in bind";
	if (listen(serverSocket, SOMAXCONN) == -1)
		throw "Error in listen";
	std::cout << "Server is listening on port " << port << '\n';
}

void removeClient(Server& server, int fd)
{
	std::map<int, Client> &clients = server.getClients();
	std::map<int, Client>::iterator it;
	Client *client = NULL;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->first == fd)
		{
			client = &it->second;
			clients.erase(it);
			break ;
		}
	}

	std::map<std::string, Channel> &channels = server.getChannels();
	std::map<std::string, Channel>::iterator __it;

	if (!client)
		return ;
	for (__it = channels.begin(); __it != channels.end(); __it++)
	{
			__it->second.removeMember(*client);
	}
}

void Server::connect_client(Server &server)
{
	struct pollfd new_poll;
	new_poll.fd = server.getSock();
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	fds.push_back(new_poll);
	while (Server::signal_received_flag == false)
	{
		if ((poll(&fds[0], fds.size(), 0) < 0) && (Server::signal_received_flag == false))
			throw "Error: poll failed";
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
			fds.push_back(new_client);
			Client client;
			client.setSocket(clientSocket);
			client.setClientIp(clientSocket);
			client.setHost(inet_ntoa(clientAddress.sin_addr));
			clients[clientSocket] = client;
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
					perror("recv");
					continue;
				}
				if (bytesRead == 0)
				{
					std::cout << "Client " << fds[i].fd << " disconnected." << '\n';
					removeClient(server, fds[i].fd);
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					i--;
					continue;
				}
				buffer[bytesRead] = '\0';
				std::string message(buffer);
				if (message.empty())
					continue;
				clients[fds[i].fd].appendToBuffer(message);
				std::string clientBuffer = clients[fds[i].fd].getBuffer();
				size_t pos = 0;
				while ((pos = clientBuffer.find('\n', pos)) != std::string::npos)
				{
					std::string completeMessage = clientBuffer.substr(0, pos + 1);
					std::cout << "Client " << fds[i].fd << " sent: " << completeMessage << '\n';
					parse_message(completeMessage, clients[fds[i].fd], server);
					clientBuffer.erase(0, pos + 1);
					pos = 0;
				}
				clients[fds[i].fd].clearBuffer();
				if (!clientBuffer.empty())
					clients[fds[i].fd].appendToBuffer(clientBuffer);
			}
		}
	}
	close_allfds();
}
