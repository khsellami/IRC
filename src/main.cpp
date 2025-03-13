
#include "../include/Server.hpp"
#include "../include/Client.hpp"
void d()
{
	system("lsof ircserv");
}
int main(int ac, char** av)
{
	// atexit(d);
	if (ac != 3 || !av[1][0] || !av[2][0])
		return (1);
	Server server(std::atoi(av[1]), std::string(av[2]));
	try
	{
		signal(SIGPIPE, SIG_IGN);
		signal(SIGINT, Server::SignalHandler);//
		signal(SIGQUIT, Server::SignalHandler);//
		server.run();
		server.connect_client(server);
		close(server.getSock());
		return (0);
	}
	catch(const char *e)
	{
		server.close_allfds();//
		std::cout << "Exception : " << e << '\n';
	}
}

