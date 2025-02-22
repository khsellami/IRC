

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <cerrno>
#include <cstring>
#include "Server.hpp"
#include "User.hpp"
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

#endif