

#include "../include/Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

Server::Server(int port, const std::string &password) : _port(port), _password(password) {}

Server::~Server()
{
    close(_serverSocket);
}

void Server::setup_server()
{
    int opt = 1;
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1) {
        exit(1);
    }

    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) {
        exit(1);
    }

    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1) {
        exit(1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
        exit(1);

    if (listen(_serverSocket, MAX_CLIENTS) == -1)
        exit(1);

    pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    _pollFds.push_back(serverPoll);

    std::cout << "Serveur IRC en écoute sur le port " << _port << std::endl;
}

void Server::run() {
    while (true) {
        if (poll(&_pollFds[0], _pollFds.size(), -1) == -1) {
            exit(1);
        }

        for (size_t i = 0; i < _pollFds.size(); i++) {
            if (_pollFds[i].revents & POLLIN) {
                if (_pollFds[i].fd == _serverSocket) {
                    acceptNewClient();
                } else {
                    handleClientMessage(_pollFds[i].fd);
                }
            }
        }
    }
}

void Server::acceptNewClient() {
    int clientSocket = accept(_serverSocket, NULL, NULL);
    if (clientSocket == -1) {
        return;
    }

    _pollFds.push_back((pollfd){clientSocket, POLLIN, 0});
    _clients.insert(std::make_pair(clientSocket, Client(clientSocket)));

    std::cout << "Nouveau client connecté: " << clientSocket << std::endl;
}

void Server::handleClientMessage(int clientSocket) {
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    
    if (bytesReceived <= 0) {
        removeClient(clientSocket);
    } else {
        buffer[bytesReceived] = '\0';
        std::cout << "Message reçu: " << buffer << std::endl;

        processCommand(clientSocket, std::string(buffer));
    }
}

void Server::processCommand(int clientSocket, const std::string& message) {
    Client& client = _clients[clientSocket];

    if (message.substr(0, 5) == "USER " && client.getUsername().empty()) {
        client.setUsername(message.substr(5));
        std::cout << "Username set to " << client.getUsername() << std::endl;
        return;
    }

    if (message.substr(0, 5) == "NICK " && client.getNickname().empty()) {
        client.setNickname(message.substr(5));
        std::cout << "Nickname set to " << client.getNickname() << std::endl;
        return;
    }

    if (message == _password) {
        authenticateClient(clientSocket);
        return;
    }

    std::cout << "Received command: " << message << std::endl;
}

void Server::authenticateClient(int clientSocket) {
    Client& client = _clients[clientSocket];

    if (client.getUsername().empty() || client.getNickname().empty()) {
        send(clientSocket, "ERROR: Username and Nickname are required.\r\n", 42, 0);
        return;
    }

    std::string welcomeMessage = "Welcome, " + client.getNickname() + "!\r\n";
    client.sendMessage(welcomeMessage);
    std::cout << "Client " << clientSocket << " authenticated." << std::endl;
}

void Server::removeClient(int clientSocket) {
    close(clientSocket);
    _clients.erase(clientSocket);

    for (size_t i = 0; i < _pollFds.size(); i++) {
        if (_pollFds[i].fd == clientSocket) {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
    std::cout << "Client déconnecté.\n";
}
