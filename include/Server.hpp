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
<<<<<<< HEAD
=======
void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client> clients);
>>>>>>> 2aaf91b3135d0bc464fdbe2f564e6bc7dedd5b4c

void parse_message(const std::string &msg1, Client &client, const char* password, std::map<int , Client> clients, Server &server, std::map<std::string, Channel> channels);
void handle_topic(Server &server, Client &client,std::map<int , Client> clients, Msj msj, std::string msg, std::map<std::string, Channel> channels);
void handle_join(Server &server, Client &client, Msj &msj);
void handle_invite(Server &server, Client &client, Msj &msj);
void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client> clients);
#endif

