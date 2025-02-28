#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include <map>
#include "Client.hpp"
#include <algorithm>
#define MAX_CHANNELS 10

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
		std::vector<Client> operators;
		//////////MODE//////////
		std::string key;
		bool is_hasKey;
		bool inviteOnly;
		bool topicRestricted;

	public:
		Channel();
		Channel(std::string name);
		//getters
		bool getTopicRestriction();
		std::string  getName();
		std::string getTopic(){return topic;}
		std::vector<Client> getMembers();
		//
		//some MODE member functions
		void setTopicRestriction(bool status);
		std::string getModeString();
		void __setOperator(std::string Nickname, bool AddorRemove);
		//setters
		void setTopic(std::string topic);
		void setName(std::string name);
		bool isInvited(Client &client)
		{
			for (size_t i = 0; i < invited.size(); i++)
			{
				if (invited[i].getNickName() == client.getNickName())
					return true;
			}
			return false;
		}
		void setOperator(Client &client);
		bool isOperator(Client &client)
		{
			for (size_t i = 0; i < operators.size(); i++)
			{
				if (operators[i].getNickName() == client.getNickName())
					return true;
			}
			return false;
		}
		bool iSInviteOnly();
        bool hasKey();
		std::string getKey();
		std::string getUserList();
		bool isFull(){return members.size() >= 10;}
		void setKey(std::string newKey);
		void removeKey();
        void setInviteOnly(bool status);
		void broadcast(const std::string &message);
		bool isBanned(Client &client);
		void addMember(Client &client);
		bool isMember(Client &client);
		void invite(Client &client);

};

#endif

