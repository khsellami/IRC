
#include "../include/header.hpp"
#include "../include/Reply.hpp"

void handle_topic(Server &server, Client &client, Msj msj)
{
	std::map<std::string, Channel> channels = server.getChannels();
	if (msj.args.size() < 2)
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(msj.args[0]));
		return;
	}
	std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
	std::map<std::string, Channel>::iterator ch_it = channels.find(channel_name);
	if (ch_it == channels.end()) 
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return;
	}
	Channel ch = ch_it->second;
	if (!ch.isMember(client))
	{
		client.sendMessage(ERR_NOTONCHANNEL(client.getName(), channel_name));
		return ;
	}
	if (msj.args.size() < 3)
	{
		if (ch.getTopic().empty())
		{
			client.sendMessage(RPL_NOTOPIC(client.getName(),channel_name));
			return;
		}
		else
		{
			client.sendMessage(RPL_TOPIC(client.getName(),channel_name,ch.getTopic()));
			return ;
		}
	}
	else
	{
		if (!ch.isOperator(client) && ch.getTopicRestriction())
		{
			client.sendMessage(ERR_CHANOPRIVSNEEDED(channel_name));
			return ;
		}
		std::string newTopic;
		newTopic = geting_message(msj.orig_msg);
		if (newTopic.empty())
			newTopic = msj.args[2];
		server.getChannels()[channel_name].setTopic(newTopic);
		std::string rpl = RPL_TOPIC(client.getName(), channel_name, newTopic);
		broadcastMessage(client, ch, rpl);
	}
}
