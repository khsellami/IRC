
#include "../include/header.hpp"
#include "../include/Reply.hpp"

#define MAX_TARGETS 10

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

void send_channel(std::string recv, Server &server, Client &client, std::string message)
{
	std::string channel_name;
	channel_name = recv.substr(1);
	std::map<std::string, Channel>::iterator ch_it = server.getChannels().find(channel_name);
	if (ch_it == server.getChannels().end()) 
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
		return;
	}
	Channel ch = ch_it->second;
	if (!ch.isMember(client))
	{
		client.sendMessage(ERR_CANNOTSENDTOCHAN(channel_name));
		return ;
	}
	std::string rpl = RPL_PRIVMSG(client.getNickName(), channel_name, message);
	broadcastMessage(client,ch, rpl);

}

void send_user(std::string recv, Server &server, std::string message, Client &client)
{
	Client *c = server.getClientByName(recv);
	if (c)
	{
		if (message.find("\001DCC") != std::string::npos)
		{
			handle_dcc_message(message, client, *c, server);
			return;
		}
		else
		{
			//check if the target is the bot, add the nick name of the sender in the first of the message

			if (recv == "bot")
			{
				std::string messageWithNick = client.getNickName() + " " + message;
				c->sendMessage(messageWithNick);
				return ;
			}
			else
			{
				std::string rpl= RPL_PRIVMSG(client.getNickName(), recv, message);
				c->sendMessage(rpl);
			}
		}
	}
	else
		client.sendMessage(ERR_NOSUCHNICK(recv));
}

std::string extract_msg(Msj msj)
{
	std::string message;
	if (msj.args[2][0] == ':')
		message = geting_message(msj.orig_msg);
	else if (msj.args.size() == 3)
		message = msj.args[2];
	return message;
}

bool handle_dcc_message(std::string message, Client &sender, Client &receiver, Server &server)
{
	(void)server;
    // Check if this is a DCC message (CTCP format with \001DCC)
    if (message.find("\001DCC") == 0 && message.find("\001", 1) != std::string::npos) {
        // This is a DCC message - just relay it to the target client
        std::string fullMessage = ":" + sender.getNickName() + "!" + sender.getHostname() + " PRIVMSG " + receiver.getNickName() + " :" + message + "\n";
        receiver.sendMessage(fullMessage);
        return true;
    }
    return false;
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
	std::vector<std::string> receivers;
	receivers = extract_recv(msj);
	if (receivers.size() > MAX_TARGETS)
	{
		client.sendMessage(ERR_TOOMANYTARGETS());
    	return;
	}
	std::string message = extract_msg(msj);
	if (message.empty())
	{
		client.sendMessage(ERR_NOTEXTTOSEND());
		return ;
	}
	for (size_t i=0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#' || receivers[i][0] == '&')
			send_channel(receivers[i], server, client,message);
		else
			send_user(receivers[i], server, message, client);
	}
}
