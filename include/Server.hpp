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
#include "Msj.hpp"
#include "Reply.hpp"

class Server
{
	private:
		int 		port;
		int			serverSocket;
		const char* password;
		std::vector<struct pollfd > fds;
		std::map<int , Client> clients;
		std::map<std::string, Channel> channels;

	public:
		Server();
		void	run();
		Server(int port, const char* password);
		int getSock() const;
		void connect_client(Server &server);
		std::map<std::string, Channel> getChannels() const;
		void addChannel(std::string name, Channel channel);
		Client* getClientByName(const std::string &name);
};

#endif

