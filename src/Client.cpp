
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>

#include "../include/Server.hpp"
#include "../include/Client.hpp"

void Client::connect_client(Server &server)
{

	fds[0].fd = server.getSock();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	numClients = 0;

	while (true)
	{
		int ready = poll(fds, numClients + 1, -1);
		if (ready == -1)
		{
			perror("poll");
			break;
		}
		if (fds[0].revents & POLLIN)
		{
			if (numClients < MAX_CLIENTS)
			{
				User user;
				struct sockaddr_in clientAddress;
				
				socklen_t clientAddressLength = sizeof(clientAddress);
				int clientSocket = accept(server.getSock(), (struct sockaddr *)&clientAddress, &clientAddressLength);
				user.setSocket(clientSocket);
				user.setHost(inet_ntoa(clientAddress.sin_addr));
				fillUsers(user);
				if (clientSocket != -1)
				{
					numClients++;
					fds[numClients].fd = clientSocket;
					fds[numClients].events = POLLIN;
					fds[numClients].revents = 0;
				}
				else
					perror("accept");
			}
		}
		//handle messages coming from Clients
		for (int i = 1; i <= numClients; i++)
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
				//client disconnect		
				if (bytesRead == 0)
				{
					std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
					close(fds[i].fd);
					for (int j = i; j < numClients; j++)
						fds[j] = fds[j + 1];
					numClients--;
					continue;
				}
				buffer[bytesRead] = '\0';
				std::string message(buffer);
				std::cout << "the Client send the message : " << message << '\n';
				// parse_message(message);//parse message
				// handle_command(message);//handle command
				}
			}
	}
}
