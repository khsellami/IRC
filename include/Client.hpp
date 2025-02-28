#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <set>
class Client 
{
	private:
		int			fd;
		std::string	host;

		//////////AUTHENTIFICATE//////////
		std::string nickname;
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		bool 		is_auth;
		//////////INVITE//////////
		int Mode_in_channel;
		std::set<std::string> joinedChannels;
	public:
	//////////AUTHENTIFICATE//////////
		bool is_PASS;
		bool is_NICK;
		bool is_USER;
		bool has_received;
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
		void setHostname(std::string Hostname);
		void setServername(std::string Servername);
		void setRealname(std::string Realname);
		void setIs_auth(bool value);
		//getters
		std::string getHostname();
		std::string getRealname();
		std::string getServername();
		int getSocket();
		std::string getNickName();
		bool getIs_auth();
		std::string getName();
		void sendMessage(const std::string &message);
		std::string getPrefix();
		std::set<std::string> getJoinedChannels();
};

#endif