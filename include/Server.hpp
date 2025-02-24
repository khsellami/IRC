#ifndef Server_HPP
#define Server_HPP

#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	private:
		int 		port;
		int			serverSocket;
		const char* password;
		std::vector<struct pollfd > fds;
		//containers client authentifiees
		std::map<int , Client> clients;
		//container channel existant in the server
		std::map<std::string, Channel> channels;

	public:
		void	run();
		Server(int port, const char* password);
		int getSock() const;
		void connect_client(Server &server);	
};

#endif

