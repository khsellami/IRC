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
		//containers client authentifiees
		std::map<int , Client> clients;
		//container channel existant in the server
		std::map<std::string, Channel> channels;

	public:
		Server(){
			port = 0;
			serverSocket = 0;
			password = nullptr;
		}
		std::map<std::string, Channel> getChannels() const{
			return channels;
		}
		void	run();
		Server(int port, const char* password);
		int getSock() const;
		void connect_client(Server &server);	
};
void parse_message(const std::string &msg1, Client &client);
void handle_join(Client &client, Msj &msj);
#endif

