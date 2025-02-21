

#include "../include/Client.hpp"
#include <sys/socket.h>
#include <cstring>

Client::Client(int socket) : _socket(socket) {}

Client::~Client() {
    close(_socket);
}

int Client::getSocket() const {
    return _socket;
}

std::string Client::getNickname() const {
    return _nickname;
}

std::string Client::getUsername() const {
    return _username;
}

void Client::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void Client::setUsername(const std::string &username) {
    _username = username;
}

void Client::sendMessage(const std::string &message) {
    std::string formattedMessage = message + "\r\n";
    send(_socket, formattedMessage.c_str(), formattedMessage.length(), 0);
}
