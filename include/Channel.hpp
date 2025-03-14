#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <map>
#include "Client.hpp"

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
		size_t limit;
		bool is_limit;
	public:
		Channel();
		Channel(std::string name);
		//getters
		bool getTopicRestriction();
		std::string  getName();
		std::string getTopic(){return topic;}
		std::vector<Client> getMembers();
		//
		//Kick command
		void removeMember(Client &client);
		//some MODE member functions
		void setTopicRestriction(bool status);
		std::string getModeString();
		void __setOperator(std::string Nickname, bool AddorRemove);
		//setters
		void setTopic(std::string topic);
		void setName(std::string name);
		bool isInvited(Client &client);
		void setOperator(Client &client);
		bool isOperator(Client &client);
		bool iSInviteOnly();
        bool hasKey();
		std::string getKey();
		std::string getUserList();
		bool isFull();
		void setKey(std::string newKey);
		void removeKey();
        void setInviteOnly(bool status);
		void setLimit(size_t newLimit);
		void removeLimit();
		void broadcast(const std::string &message);
		bool isBanned(Client &client);
		void addMember(Client &client);
		bool isMember(Client &client);
		void invite(Client &client);
		bool isOwner(Client& client);

};

#endif

