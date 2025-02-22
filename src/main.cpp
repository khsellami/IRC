
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <stdlib.h>

int main(int ac, char** av)
{
	if(ac != 3){return 1;}
	Server server(std::atoi(av[1]), av[2]);
	server.run();
	Client client;
	client.connect_client(server);
    close(server.getSock());
    return 0;
}
