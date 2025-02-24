
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include<sstream>

#include "../include/Server.hpp"
#include "../include/Client.hpp"

Client::Client()
{
	auth = false;
	auth_count = 0;
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

            if (clientSocket == -1) {
                perror("accept");
                continue;  // Retry accepting new connections
            }

            struct pollfd new_client;
            new_client.fd = clientSocket;
            new_client.events = POLLIN;
            new_client.revents = 0;

            Client Client;
            Client.setSocket(clientSocket);
            Client.setHost(inet_ntoa(clientAddress.sin_addr));

            fds.push_back(new_client);
            std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;
        }

        // Check all connected clients for activity
        for (size_t i = 1; i < fds.size(); i++)  // Start at 1 to avoid the server socket itself
        {
            if (fds[i].revents & POLLIN) {
                char buffer[1024];
                ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead == -1) {
                    perror("read");
                    continue;
                }

                // Client disconnect
                if (bytesRead == 0) {
                    std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    i--;
                    continue;
                }

                buffer[bytesRead] = '\0';
                std::string message(buffer);
                std::cout << "Client " << fds[i].fd << " sent: " << message << '\n';
				//check if the client is authe
				//parse message
            }
        }
    }
	// Cleanup: Close all file descriptors before program termination
	// for (size_t i = 0; i < fds.size(); i++) {
	// 	close(fds[i].fd);
	// }
}