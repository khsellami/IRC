
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

std::string toUpper(const std::string &str)
{
	std::string upperStr = str;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
	return upperStr;
}

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\r\0");
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, last - first + 1);
}

void parse_message(const std::string &msg1, Client &client, Server &server)
{
	Msj msj;
	std::string msg = trim(msg1);
	std::stringstream  ss(msg);
	std::string word;
	while (ss >> word)
		msj.args.push_back(word);
	msj.orig_msg = msg;
	if (msj.args.empty())
		return ;
	handle_authentification(client, msj, server);
	if (client.getIs_auth() == false)
		return ;
	if (msj.args.size() > 0 && toUpper(msj.args[0]) == "JOIN")
	{
		handle_join(server, client, msj);
	}
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "INVITE")
	{
		handle_invite(server, client, msj);
    }
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "TOPIC")
	{
		handle_topic(server, client, msj);
	}
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "PRIVMSG")
	{
		handle_privmsg(server, client, msj);
	}
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "MODE")
	{
		handleChannelMode(client, msj, server);
	}
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "KICK")
	{
		handleKickCommand(client, msj, server);
	}
}
