// IRCServer.cpp

#include "IRCServer.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>  // For fcntl()
#include <cstring>  // For strerror()

// Constructor
IRCServer::IRCServer(int port, const std::string& password) {
    this->port = port;
    this->password = password;

    // Initialize the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 10) == -1) {
        std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
        close(server_fd);
        exit(1);
    }
}

// Definition of start() function
void IRCServer::start() {
    while (true) {
        handleNewConnection();  // Make sure handleNewConnection is being called here
    }
}

// Definition of handleNewConnection()
void IRCServer::handleNewConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);

    if (client_fd == -1) {
        std::cerr << "Error accepting new connection: " << strerror(errno) << std::endl;
        return;
    }

    // Log new connection
    std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr)
              << ":" << ntohs(client_addr.sin_port) << " (fd: " << client_fd << ")" << std::endl;

    // Set client socket to non-blocking (optional)
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // Add client to the list of connected clients
    clients.push_back(client_fd);

    // Send welcome message to the client
    sendMessage(client_fd, "Welcome to the IRC server!\n");
}

// Definition of sendMessage()
void IRCServer::sendMessage(int client_fd, const std::string& message) {
    ssize_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);

    if (bytes_sent == -1) {
        std::cerr << "Error sending message to client (fd: " << client_fd
                  << "): " << strerror(errno) << std::endl;
    } else {
        std::cout << "Sent " << bytes_sent << " bytes to client (fd: " << client_fd << ")." << std::endl;
    }
}
