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
		void addChannel(std::string name, Channel channel){
			channels[name] = channel;
		}
		Client* getClientByName(const std::string &name) {
			for (size_t i = 0; i < clients.size(); i++) {
				if (clients[i].getName() == name) {
					return &clients[i];
				}
			}
			return NULL;
		}
		void	run();
		Server(int port, const char* password);
		int getSock() const;
		void connect_client(Server &server);	
};
void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client> clients);

void parse_message(const std::string &msg1, Client &client, const char* password, std::map<int , Client> clients, Server &server);
void handle_join(Server &server, Client &client, Msj &msj);
void handle_invite(Server &server, Client &client, Msj &msj);
#endif

