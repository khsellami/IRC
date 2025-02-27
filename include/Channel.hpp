#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include <map>
#include "Client.hpp"
#include <algorithm>

class Client;
class Channel
{
	private:
		std::string name_channel;
		//////////TOPIC//////////
		std::string topic;
		//////////JOIN//////////
		std::vector<Client> members;
		std::vector<Client> invited;
		std::vector<std::string> bannedUsers;
		//////////MODE//////////
		std::vector<Client> operators;
		std::string key;
		bool inviteOnly;
		bool t;

	public:
	//constructors
		Channel();
		Channel(std::string name);
		//getters
		bool getT();
		std::string  getName();
		std::string getTopic(){return topic;}
		std::vector<Client> getMembers();
		//setters
		void setTopic(std::string topic);
		void setName(std::string name);
		bool isInvited(Client &client){return std::find(invited.begin(), invited.end(), client) != invited.end();}
		// bool isOperator(const Client &client) const { return std::find(operators.begin(), operators.end(), client) != operators.end(); }
		void setOperator(Client &client);
		bool isOperator(Client &client){return std::find(operators.begin(), operators.end(), client) != operators.end();}
		bool iSInviteOnly();
        bool hasKey();
		std::string getKey();
		std::string getUserList();
		// bool iSInviteOnly(){return t;}
		void setKey(std::string newKey);
        void setInviteOnly(bool status);
		void broadcast(const std::string &message);
		bool isBanned(Client &client);
		void addMember(Client &client);
		bool isMember(Client &client);
		void invite(Client &client);

};

#endif

