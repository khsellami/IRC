#ifndef Server_HPP
#define Server_HPP

#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include "Server.hpp"
#include <vector>

class Server
{
	private:
		int 		port;
		int			serverSocket;
		const char* password;
		std::vector<struct pollfd > fds;

	public:
		void	run();

		Server(int port, const char* password)
		{
			this->port = port;
			this->password = password;
		}

		int getSock() const
		{
			return serverSocket;
		}
		void connect_client(Server &server);	
};

#endif