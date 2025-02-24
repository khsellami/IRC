#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client 
{
	private:
		int			fd;
		std::string	host;
		std::string nickname;
		std::string username;
		//AUTHENTIFICATE
		bool is_auth;
		// int auth_count;
		//JOIN
		// bool is_op;
	public:
		Client();

		void setSocket(int fd);
		void setHost(const std::string &host);
		int getSocket();
		int getIs_auth();
		std::string getName(){
			return this->nickname;
		}
};

#endif
