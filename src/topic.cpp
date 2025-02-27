
#include "../include/header.hpp"
#include "../include/Reply.hpp"

void handle_topic(Server &server, Client &client, Msj msj)
{
	//DEBUG////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// std::cout << "*****************************************************************\n\n\n";
	// std::cout << "CHannels\n\n";
	// for(std::map<std::string, Channel>::iterator it = server.getChannels().begin(); it != server.getChannels().end() ;++it)
	// {
	// 	std::cout << it->first << '\n';
	// }
	//****just for debug***********************************//
	// std::vector<std::string>::iterator it = msj.args.begin();
	// while (it != msj.args.end())
	// {
	// 	std::cout << "[" << *it << "] ";
	// 	it++;
	// }
	// std::cout << '\n';
	// std::cout << "*********Inside topic Command*******************\n";
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Take the list of the channels
	//****Few arguments***********************************//
	std::map<std::string, Channel> channels = server.getChannels();
	if (msj.args.size() < 2)
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(std::string("TOPIC")));
		return;
	}

	// Extract channel name
	std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
	// Search for channel in channels map
	std::map<std::string, Channel>::iterator ch_it = channels.find(channel_name);
	//****Incorrect channel name***********************************//
	if (ch_it == channels.end()) 
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	//****channel existe***********************************//
	Channel ch = ch_it->second;

	//****if the client not a member in the channel***********************************//
	/*
	ch <<== client
	*/
	if (!ch.isMember(client))
	{
		client.sendMessage(ERR_NOTONCHANNEL(client.getName(), channel_name));
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
			//RPL_NOTOPIC
			client.sendMessage(RPL_NOTOPIC(client.getName(),channel_name));
			return;
		}
		//topic exist
		else
		{
			client.sendMessage(ch.getTopic());
			return ;
		}
	}
	//****update topic***********************************//
	else
	{
		//message not start with :
		if (msj.args[2][0] != ':')
		{
			client.sendMessage(RPL_NOTOPIC(client.getName(),channel_name));
			return ;
		}
		//No permissions
		if (!client.isOp() && !ch.getT())
		{
			client.sendMessage(ERR_CHANOPRIVSNEEDED(channel_name));
			return ;
		}
		//Has permissions
		std::string newTopic;
		newTopic = geting_message(msj.orig_msg);
		// ch.setTopic(newTopic);
		server.getChannels()[channel_name].setTopic(newTopic);
		//broadcastmessage to all members in channel expect client itself
		std::string rpl = RPL_TOPIC(client.getName(), channel_name, newTopic);
		//****Broadcast the change of the topic to All members in the channel***********************************//
		broadcastMessage(client, ch, rpl);
	}
}
