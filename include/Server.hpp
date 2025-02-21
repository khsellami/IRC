
#ifndef Server_HPP
#define Server_HPP

#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"
#include <cstdlib>

#define MAX_CLIENTS 100

class Server {
private:
    int _port;
    std::string _password;
    int _serverSocket;
    std::vector<pollfd> _pollFds;
    std::map<int, Client> _clients;

    void authenticateClient(int clientSocket);

public:
    Server(int port, const std::string &password);
    ~Server();

    void run();
    void setup_server();
    void acceptNewClient();
    void handleClientMessage(int clientSocket);
    void removeClient(int clientSocket);
    void processCommand(int clientSocket, const std::string& message);
};

#endif
