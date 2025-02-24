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

std::string getCommand(const std::string &msg)
{
    return msg.substr(0, msg.find(" "));
}

std::string getArgument(const std::string &msg)
{
    size_t pos = msg.find(" ");
    return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
}

#include <algorithm>

std::string toUpper(const std::string &str)
{
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}
std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}
void parse_message(const std::string &msg1, Client &client)
{
	Msj msj;
	(void)client;
	std::string msg = trim(msg1);
    std::cout << "The message is: " << msg << '\n';
    
    std::string command = getCommand(msg);
    std::string argument = getArgument(msg);
	std::string CMD = toUpper(command);
	// if(CMD == "JOIN")
	// 	handle_join(client, argument);

	msj.setCommand(CMD);
	msj.setArgument(argument);
	if(msj.getCommand() == "JOIN")
	{
		std::cout << "The command is: " << msj.getCommand() << '\n';
		std::cout << "The argument is: " << msj.getArgument() << '\n';
	}
	msj.SetArgs(argument);
	for (size_t i = 0; i < msj.getArgs().size(); i++)
	{
		std::cout << "The args are: " << msj.getArgs()[i] << '\n';
	}
	// if(CMD == "NICK")
	// 	handle_nick(client, argument);
	// if(CMD == "USER")
	// 	handle_user(client, argument);
	// if(CMD == "PRIVMSG")
	// 	handle_privmsg(client, argument);
	// if(CMD == "QUIT")
	// 	handle_quit(client, argument);
	// if(CMD == "TOPIC")
	// 	handle_topic(client, argument);
	// if(CMD == "INVITE")
	// 	handle_invite(client, argument);
	
}


void handle_authentification(Client &client, std::string message)
{
	//check if pass is the first commande then terminate
	//handle nick user
	(void)message;
	std::cout << "Authenticating client: " << client.getSocket() << '\n';
	// std::cout << "The message is : " << message << '\n';
	//make the bool is_auth true after authentification success

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
				if (clients.find(fds[i].fd) != clients.end())
				{
					handle_authentification(clients[fds[i].fd], message);
					//if the client is authentificate
					// if (client.getIs_auth() == true)
					parse_message(message, clients[fds[i].fd]);
				}
			}
		}
	}
	// Cleanup: Close all file descriptors before program termination
	for (size_t i = 0; i < fds.size(); i++) {
		close(fds[i].fd);
	}
}

