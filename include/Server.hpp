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

// #include "Client.hpp"
#include "Server.hpp"
#include <vector>
// class Client;
class User;
class Server;

class Server
{
	private:
		int 		port;
		int			serverSocket;
		const char* password;

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
};

#endif