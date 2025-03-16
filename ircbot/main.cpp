#include "bot.hpp"


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: <Server IP> <Server Port> <Server Password>" << std::endl;
        return 1;
    }
    std::string ip(argv[1]);
    std::string port(argv[2]);
    std::string password(argv[3]);
    Bot bot;
    bot.setSocketFd(socket(AF_INET, SOCK_STREAM, 0));
    if (bot.getSocketFd() < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }
    bot.setServerInfos(ip, port);
    bot.connectToServer();
    bot.registerBot(password);
    while (true)
    {
        char buffer[1024];
        int bytes_received = recv(bot.getSocketFd(), buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            std::cerr << "Connection closed" << std::endl;
            break;
        }
        std::string response(buffer, bytes_received);
        if (!bot.isRegistred())
            bot.checkRegistred(response);
        if (bot.isRegistred())
            bot.handle_message(response);
    }
    close(bot.getSocketFd());
}