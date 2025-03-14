#ifndef Server_HPP
#define Server_HPP

#include <cstdio>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include "Msj.hpp"
#include "Reply.hpp"
#include <csignal>

class Server
{
	private:
	std::vector<struct pollfd > fds;
	std::map<int , Client> clients;
	std::map<std::string, Channel> channels;
	
	int 		port;
	std::string password;
		int			serverSocket;
	public:
		Server();
		Channel* getChannel(const std::string& channelName);
		std::string getPassword();
		void	run();
		Server(int port, std::string password);
		int getSock() const;
		void connect_client(Server &server);
		std::map<std::string, Channel> &getChannels();
		void addChannel(std::string name, Channel channel);
		Client* getClientByName(const std::string &name);
		std::map<int , Client> &getClients();
		void	close_allfds();
		static bool signal_received_flag;
		static void SignalHandler(int signum);
		std::string getHost(){return "localhost";}
		
	
};
void handleDCCSend(Client &sender, const std::string &message);

#endif

