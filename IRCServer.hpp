// IRCServer.hpp

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <string>
#include <vector>

class IRCServer {
public:
    IRCServer(int port, const std::string& password);
    void start();
    void handleNewConnection();  // Ensure this is declared here
    void sendMessage(int client_fd, const std::string& message);

private:
    int server_fd;
    int port;
    std::string password;
    std::vector<int> clients;  // Storing client fds
};

#endif
