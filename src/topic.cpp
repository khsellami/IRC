
#include "../include/header.hpp"

void handle_topic(Server &server, Client &client, Msj msj)
{
	std::cout << "CHannels\n\n";
	for(std::map<std::string, Channel>::iterator it = server.getChannels().begin(); it != server.getChannels().end() ;++it)
	{
		std::cout << it->first << '\n';
	}
	//****just for debug***********************************//
	std::vector<std::string>::iterator it = msj.args.begin();
	while (it != msj.args.end())
	{
		std::cout << "[" << *it << "] ";
		it++;
	}
	std::cout << '\n';
	std::cout << "*********Inside topic Command*******************\n";

	//Take the list of the channels
	//****Few arguments***********************************//
	std::map<std::string, Channel> channels = server.getChannels();
	if (msj.args.size() < 2)
	{
		std::cout << "The topic command needs arguments ERR_NEEDMOREPARAMS\n";
		return;
	}

	// Extract channel name
	std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
	

	// Search for channel in channels map
	std::map<std::string, Channel>::iterator ch_it = channels.find(channel_name);
	//****Incorrect channel name***********************************//
	if (ch_it == channels.end()) 
	{
		std::cout << "No such channel name ERR_NOSUCHCHANNEL\n";
		return;
	}

	//****channel existe***********************************//
	Channel ch = ch_it->second;

	//****if the client not a member in the channel***********************************//
	/*
	ch<<== client
	*/
	if (!ch.isMember(client))
	{
		std::cout << "This client not a member of channel ERR_NOTONCHANNEL\n";
		return ;
	}

	///////////////////////////////////////////////////////////////////////////////////
	//if message existe ==>>update topic
	//if message does not exist ===>>view topic

	//****view topic***********************************//
	if(msj.args.size() < 3)
	{
		//no topic provided
		if(ch.getTopic().empty())
		{
			std::cout << "No topic to view in the channel RPL_NOTOPIC\n";
			return;
		}
		//topic exist
		else
		{
			std::cout << ch.getTopic() << '\n';
			return ;
		}
	}

	//****update topic***********************************//
	else
	{
		//message not start with :
		if (msj.args[2][0] != ':')
		{
			std::cout << "The message must start with <:> RPL_NOTOPIC\n";
			return ;
		}
		//No permissions
		if (!client.isOp() && !ch.getT())
		{
			std::cout << "No permissions ERR_CHANOPRVSNEEDED\n";
			return ;
		}
		//Has permissions
		std::string newTopic;
		newTopic = geting_message(msj.orig_msg);
		std::cout << "new topic=" << newTopic << '\n';
		ch.setTopic(newTopic);
		//broadcastmessage to all members in channel expect client itself
		std::string rpl = "New topic changed RPL_TOPIC\n";
		// std::cout << rpl << '\n';
		//****Broadcast the change of the topic to All members in the channel***********************************//
		broadcastMessage(client, ch, rpl);
	}
}
