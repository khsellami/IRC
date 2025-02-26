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
		//////////MODE//////////
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
		bool isOperator(Client &client){
			if (client.getIs_operator() == true)
				return true;
			return false;
		}
		bool isInviteOnly(){return t;}
		void addMember(Client &client);
		bool isMember(Client &client);
		void invite(Client &client);

};

#endif

