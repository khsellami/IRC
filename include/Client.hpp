#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#define PASS_VALIDATED 0
#define NICK_VALIDATED 1
#define USER_VALIDATED 2
#define PASS_NEEDED_NICK_VALIDATED -1
#define PASS_NEEDED_USER_VALIDATED -2
#define JUST_PASS_NEEDED_TO_AUTH -3

class Client 
{
	private:
		int			fd;
		std::string	host;
		std::string nickname;
		std::string username;
		//AUTHENTIFICATE
		bool is_auth;
		//JOIN
		// bool is_op;
	public:
		bool is_PASS;
		bool is_NICK;
		bool is_USER;
		Client();

		// setters:
		void setSocket(int fd);
		void setHost(const std::string &host);
		void setNickName(std::string Nick);
		void setUserName(std::string User);
		void setIs_auth(bool value);

		//getters:
		int getSocket();
		std::string getNickName();
		bool getIs_auth();
		std::string getName(){
			return this->nickname;
		}
};

#endif
