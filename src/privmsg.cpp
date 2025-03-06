
#include "../include/header.hpp"
#include "../include/Reply.hpp"

#define MAX_TARGETS 10

//****Extract Receivers***********************************//
std::vector<std::string> extract_recv(Msj msj)
{
	std::vector<std::string> receivers;
	std::string targetList = msj.args[1];
	std::stringstream ss(targetList);
	std::string target;
	while(std::getline(ss, target, ','))
	{
		if (!target.empty())
			receivers.push_back(target);
	}
	return receivers;
}

//****send Channel***********************************//
void send_channel(std::string recv, Server &server, Client &client, std::string message)
{
	std::string channel_name;
	channel_name = recv.substr(1);
	// Search for channel in channels map
	std::map<std::string, Channel>::iterator ch_it = server.getChannels().find(channel_name);
	//****Incorrect channel name***********************************//
	if (ch_it == server.getChannels().end()) 
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	//****channel existe***********************************//
	Channel ch = ch_it->second;
	if (!ch.isMember(client))
	{
		client.sendMessage(ERR_CANNOTSENDTOCHAN(channel_name));
		return ;
	}
	std::string rpl = RPL_PRIVMSG(client.getNickName(), channel_name, message);
	broadcastMessage(client,ch, rpl);

}

//****Send User***********************************//
void send_user(std::string recv, Server &server, std::string message, Client &client)
{
	Client *c = server.getClientByName(recv);
	if (c)
	{
		std::string rpl = RPL_PRIVMSG(client.getNickName(), recv, message);
		c->sendMessage(rpl);
	}
	else
		client.sendMessage(ERR_NOSUCHNICK(recv));
}

//****Extract Message***********************************//
std::string extract_msg(Msj msj)
{
	std::string message;
	if (msj.args[2][0] == ':')
		message = geting_message(msj.orig_msg);
	else if (msj.args.size() == 3)
		message = msj.args[2];
	return message;
}

void handle_privmsg(Server &server, Client &client, Msj msj)
{
	if (msj.args.size() < 2)
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(msj.args[0]));
		return ;
	}
	if (msj.args.size() < 3)
	{
		client.sendMessage(ERR_NOTEXTTOSEND());
		return ;
	}
	//****Extract Receivers***********************************//
	std::vector<std::string> receivers;
	receivers = extract_recv(msj);
	if (receivers.size() > MAX_TARGETS)
	{
		client.sendMessage(ERR_TOOMANYTARGETS());
    	return;
	}
	//****Extract Message***********************************//
	std::string message = extract_msg(msj);
	if (message.empty())
	{
		client.sendMessage(ERR_NOTEXTTOSEND());
		return ;
	}
	//****SEND Message***********************************//
	for (size_t i=0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#' || receivers[i][0] == '&')
			send_channel(receivers[i], server, client,message);
		else
			send_user(receivers[i], server, message, client);
	}
}
