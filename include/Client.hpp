#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#define PASS_VALIDATED 0
#define NICK_VALIDATED 1
#define USER_VALIDATED 2
#define PASS_NEEDED_NICK_VALIDATED -1
#define PASS_NEEDED_USER_VALIDATED -2
#define JUST_PASS_NEEDED_TO_AUTH -3

#define OPERATOR 1
#define INVITED 2
#define MEMBER 3

class Client 
{
	private:
		int			fd;
		std::string	host;

		//////////AUTHENTIFICATE//////////
		std::string nickname;
		std::string username;
		bool is_auth;
		//////////INVITE//////////
		int Mode_in_channel;
		bool is_op;
	public:
	//////////AUTHENTIFICATE//////////
		bool is_PASS;
		bool is_NICK;
		bool is_USER;
		std::string messageToSend;
		bool operator==(const Client &other) const {
        return this->nickname == other.nickname; // Comparaison par pseudo
    	}


		Client();
		//setters
		void setSocket(int fd);
		void setHost(const std::string &host);
		void setNickName(std::string Nick);
		void setUserName(std::string User);
		void setIs_auth(bool value);
		//getters
		int getSocket();
		std::string getNickName();
		bool getIs_auth();
		std::string getName();
		bool isOp();
		void sendMessage(const std::string &message);
		bool getIs_operator(){
			if (is_op == true)
				return true;
			return false;
		}
		std::string getPrefix();
};

#endif
