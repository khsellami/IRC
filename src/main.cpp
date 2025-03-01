
#include "../include/Server.hpp"
#include "../include/Client.hpp"

// void ff()
// {
// 	system("lsof -c ircserv");
// }
int main(int ac, char** av)
{

	// atexit(ff);
	if(ac != 3){return 1;}
	Server server(std::atoi(av[1]), std::string(av[2]));
	server.run();
	server.connect_client(server);
    close(server.getSock());
    return 0;
}
