#include <cstdlib> 
#include "../include/Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    Server server(port, password);
    server.setup_server();
    server.run();

    return 0;
}
