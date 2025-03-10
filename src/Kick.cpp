#include "../include/header.hpp"


void handleKickCommand(Client &client, Msj msj, Server &server)
{
	if (msj.args.size() < 3)
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(msj.args[0]));
		return ;
	}
	std::string channel_name = msj.args[1];
	if (channel_name[0] != '#' && channel_name[0] != '&')
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return ;
	}
	channel_name =  msj.args[1].substr(1);
	std::string target = msj.args[2];
	std::string reason = msj.args.size() > 3 ? geting_message(msj.orig_msg) : "Kicked";
	// i just check first if the channel exist
	Channel *channel = server.getChannel(channel_name);
	if (!channel)
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return ;
	}
	// just operators can kick members
	if (!channel->isOperator(client))
	{
		client.sendMessage(ERR_CHANOPRIVSNEEDED(channel_name));
		return ;
	}
	Client *targetClient = server.getClientByName(target);
	if (!targetClient)
	{
		client.sendMessage(ERR_NOSUCHNICK(target));
		return ;
	}
	if (!channel->isMember(*targetClient))
	{
		client.sendMessage(ERR_USERNOTINCHANNEL(target, channel_name));
		return ;
	}
	channel->removeMember(*targetClient);
	targetClient->sendMessage(":" + client.getNickName() + " KICK " + channel_name + " " + target + " :" + reason + "\n");
	broadcastMessage(client, *channel, ":" + client.getNickName() + " KICK " + channel_name + " " + target + " :" + reason + "\n");
}
