
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <cctype>

int is_valid_port(const char *port_str)
{
	for (int i = 0; port_str[i]; i++)
    {
        if (!std::isdigit(port_str[i]))
        {
            std::cerr << "Error Port must contain only digits (0-9).\n";
            return false;
        }
    }
    int port = std::atoi(port_str);
    if (port < 1 || port > 65535)
    {
        std::cerr << "Error Invalid port number\n";
        return (0);
    }
    return (1);
}


int main(int ac, char** av)
{
	if (ac != 3 || !av[1][0] || !av[2][0] || !is_valid_port(av[1]))
		return (1);
	Server server(std::atoi(av[1]), std::string(av[2]));
	try
	{
		signal(SIGPIPE, SIG_IGN);
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		server.run();
		server.connect_client(server);
		return (0);
	}
	catch(const char *e)
	{
		server.close_allfds();
		std::cout << "Exception : " << e << '\n';
	}
}
