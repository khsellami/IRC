
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
	{
		std::cout << "Server <" << serverSocket << "> disconnected" << std::endl;
		std::cout << "All clients disconnected" << std::endl;
		close(serverSocket);
	}
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
	// int opt = 1;
	///********************* create socket *********************************************///
	//AF_INET ==>>> Adresse IPv4
	//SOCK_STREAM ==>>> Mode TCP 
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw "Error when creating socket\n";
	///********************* configuration socket *********************************************///
	// if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,  (char *) &opt, sizeof(opt))) //reutiliser adresse apres fermuture
	// {
	// 	close(serverSocket);
	// 	throw "Error when set socket in reuse address\n";
	// }
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) //mode non-bloquant
	{
		close(serverSocket);
		throw "Error when set socket in non bloquant\n";
	}
	///********************* bind socket *********************************************///
	//liaison de socket a une adresse
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET; //=======>>>>>>>>> Adresse IPv4
	serverAddress.sin_port = htons(port);//=======>>>>>>>>> port du serveur
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //=======>>>>>>>>> Accepte les connexions sur tous les interfaces reseau
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close(serverSocket);
		throw "Error in bind\n";
	}
	///********************* socket in mode listen *********************************************///
	//SOMAXCONN ==>>> la longueur maximale de la file d’attente des connexions entrantes
	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		close(serverSocket);
		throw "Error in listen\n";
	}
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
			throw (("Error: poll failed"));
			///********************* SERVER ===>> Accept de nouvelles connexions *********************************************///
			/*
			revents peut contenir plusieurs evenements a la fois : POLLIN | POLLERR
			Verifie si le bit correspondant a POLLIN est active dans revents
			*/
			if (fds[0].revents & POLLIN)
			{
				struct sockaddr_in clientAddress;
				socklen_t clientAddressLength = sizeof(clientAddress);
				//create socket client
				int clientSocket = accept(server.getSock(), (struct sockaddr *)&clientAddress, &clientAddressLength);
				if (clientSocket == -1)
				{
					perror("accept");
					continue;
				}
				//add le nouvel client to fds to manage it by poll
				struct pollfd new_client;
				new_client.fd = clientSocket;
				new_client.events = POLLIN;
				new_client.revents = 0;
				fds.push_back(new_client);

				//Create objet client && add it to clients
				Client client;
				client.setSocket(clientSocket);
				client.setClientIp(clientSocket);
				client.setHost(inet_ntoa(clientAddress.sin_addr));
				clients[clientSocket] = client;
				std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << '\n';
			}
			///********************* CLIENTS ===>> receive message & pass it to parse_message to tarite it*********************************************///
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].revents & POLLIN)
				{
					char buffer[1024];
					ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
					//ERROR ==>> receive data
					if (bytesRead == -1)
					{
						perror("recv");
						continue;
					}
					//CLIENT disconnected
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
					///////////**** handle message buffering ****///////////
					std::string message(buffer);
					if (message.empty())
						continue;
					clients[fds[i].fd].appendToBuffer(message);//add message to buffer


					std::string clientBuffer = clients[fds[i].fd].getBuffer();
					size_t pos = 0;
					while ((pos = clientBuffer.find('\n', pos)) != std::string::npos)//check if the message completed by newline
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
