#ifndef USER_HPP
#define USER_HPP
#include <iostream>
class User
{
private:
    int         fd;
    std::string host;
    std::string nickname;
    std::string username;

public:
    User() {}

    void setSocket(int fd) { this->fd = fd; }
    void setHost(const std::string &host) { this->host = host; }
    void setNickname(const std::string &nickname) { this->nickname = nickname; }
    void setUsername(const std::string &username) { this->username = username; }

    std::string getNickname() const { return nickname; }
    std::string getUsername() const { return username; }
};



#endif