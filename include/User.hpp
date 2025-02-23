#ifndef USER_HPP
#define USER_HPP
#include <iostream>
const int MAX_CLIENTS = 20;
class User 
{
private:
    int         fd;
    std::string host;
    std::string nickname;
    std::string username;
    int     numClients;
    bool     auth;
    int      
    auth_count;

public:
    User();

    void setSocket(int fd) { this->fd = fd; }
    void setHost(const std::string &host) { this->host = host; }
    void setNickname(const std::string &nickname) { this->nickname = nickname; }
    void setUsername(const std::string &username) { this->username = username; }

    std::string getNickname() const { return nickname; }
    std::string getUsername() const { return username; }
    void connect_client(Server &server);
};



#endif