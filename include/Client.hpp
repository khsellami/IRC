#ifndef Client_HPP
#define Client_HPP
#include <iostream>

class Client 
{
private:
    int         fd;
    std::string host;
    std::string nickname;
    std::string Clientname;
    bool     auth;
    int      auth_count;

public:
    Client();

    void setSocket(int fd) { this->fd = fd; }
    void setHost(const std::string &host) { this->host = host; }
    void setNickname(const std::string &nickname) { this->nickname = nickname; }
    void setClientname(const std::string &Clientname) { this->Clientname = Clientname; }

    std::string getNickname() const { return nickname; }
    std::string getClientname() const { return Clientname; }
};



#endif