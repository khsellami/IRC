
#include "../include/Server.hpp"
#include "../include/Client.hpp"

int main(int ac, char** av)
{
	try
	{
		if (ac != 3 || !av[1][0] || !av[2][0])
			return (1);
		Server server(std::atoi(av[1]), std::string(av[2]));
		server.run();
		server.connect_client(server);
		close(server.getSock());
		return (0);
	}
	catch(const char *e)
	{
		std::cout << "Exception : " << e << '\n';
	}
}

