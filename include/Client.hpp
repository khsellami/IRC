
#ifndef Client_HPP
#define Client_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

class Client {
private:
    int _socket;
    std::string _nickname;
    std::string _username;

public:
    Client() : _socket(-1) {}
    Client(int socket);
    ~Client();

    int getSocket() const;
    std::string getNickname() const;
    std::string getUsername() const;

    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void sendMessage(const std::string &message);
};

#endif
