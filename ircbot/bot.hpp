#ifndef BOT_HPP
#define BOT_HPP
#include "../include/header.hpp"

class Bot
{
    public:
        Bot(Client &client, Server &server);
        void run();
    private:
        Client &client;
        Server &server;
};
#endif
