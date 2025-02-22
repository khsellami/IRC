

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <cerrno>
#include <cstring>
#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <map> 
#include <set>
#include <sstream>
const int MAX_CLIENTS = 20;




class Server;
class Client
{
	private:
		int numClients;
		struct pollfd fds[MAX_CLIENTS + 1];
		std::vector<User> users;

	public:

		void	fillUsers(User &user)
		{
			users.push_back(user);
		}
		void connect_client(Server &server);
		struct pollfd	getFds() const
		{
			return fds[numClients];
		}
		int getFdsNum() const
		{
			return numClients;
		}
		void setNumClients(int numClients)
		{
			this->numClients = numClients;
		}
		int	getNumClients() const
		{
			return numClients;
		}

};
void handle_nick(int client_fd, std::istringstream &iss);
void handle_user(int client_fd, std::istringstream &iss);
void handle_join(int client_fd, std::istringstream &iss);
void handle_privmsg(int client_fd, std::istringstream &iss);
void handle_kick(int client_fd, std::istringstream &iss);
void handle_join(int client_fd, std::istringstream &iss);
void handle_invite(int client_fd, std::istringstream &iss);
void handle_mode(int client_fd, std::istringstream &iss);
void handle_topic(int client_fd, std::istringstream &iss);
#endif