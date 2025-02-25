

#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector>
std::string geting_message(const std::string &msg)
{
	size_t pos = msg.find(":");
	return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
}

void handle_topic(Server &server, Client &client, std::map<int, Client> clients, 
				  Msj msj, std::string msg, std::map<std::string, Channel> channels)
{
	(void)server;
	(void)clients;

	std::cout << "Calling topic command\n";
	std::cout << "===>>Arguments\n";

	std::vector<std::string>::iterator it = msj.args.begin();
	while (it != msj.args.end())
	{
		std::cout << "[" << *it << "] ";
		it++;
	}
	std::cout << '\n';
	//just topic error
	// At least "TOPIC #ch1"
	if (msj.args.size() < 2)
	{
		std::cout << "The topic command needs arguments\n";
		return;
	}

	// Extract channel name
	std::cout << "The channel name is: " << msj.args[1] << '\n';
	std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
	std::cout << "Extracted Channel name: " << channel_name << '\n';

	// Search for channel
	std::map<std::string, Channel>::iterator ch_it = channels.find(channel_name);
	if (ch_it == channels.end()) 
	{
		std::cout << "No such channel name\n";
		return;
	}

	// Get channel reference
	Channel *ch = &(ch_it->second);

	// ✅ Now you can work with `ch`
	std::cout << "Channel found: " << channel_name << '\n';

	//if message existe ==>>update topic
	//if message does not exist ===>>view topic

	if(msj.args.size() < 3)//view 
	{
		if(ch->getTopic().empty())//no topic
		{
			std::cout << "No topic to view in the channel\n";
			return;
		}
		else
		{
			std::cout << ch->getTopic() << '\n';
			return ;

		}

	}
	else//update
	{
		//no permissions
		if (!client.isOp() && !ch->getT())
		{
			std::cout << "No permissions T\n";
			return ;
		}
		//has permissions
		std::string newTopic;
		newTopic = geting_message(msg);
		ch->setTopic(newTopic);
		//broadcastmessage to all members in channel expect client itself


	}

}
