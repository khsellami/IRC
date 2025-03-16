#ifndef BOT_HPP
#define BOT_HPP
#include "../include/header.hpp"

class Bot
{
    public:
        Bot();
        static const std::string quotes[20];

        void setSocketFd(int fd);
        int getSocketFd();
        void setServerInfos(std::string ip, std::string port);
        void connectToServer();
        void registerBot(std::string password);
        void checkRegistred(std::string response);
        void setRegistred(bool registred);
        bool isRegistred();
        void handle_message(std::string message);

    private:
        int fd;
        struct sockaddr_in server_addr;
        bool registred;
};
#endif
