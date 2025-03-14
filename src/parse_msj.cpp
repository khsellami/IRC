
#include "../include/header.hpp"


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

void parse_bot_command(Msj &msj, Client &client, Server &server)
{
	std::string messageWithNick = client.getNickName() + " " + msj.orig_msg;
	Client *bot = server.getClientByName("bot");
	if (!bot)
	{
		std::string message = "bot Not found";
		std::cerr << message << std::endl;
		send(client.getSocket(), message.c_str(), message.size(), 0);
		return ;
	}
	if (msj.args[0].find("!HELP") != std::string::npos || msj.args[0].find("!TIME") != std::string::npos 
	|| msj.args[0].find("!QUOTE") != std::string::npos)
	{
		if (send(bot->getSocket(), messageWithNick.c_str(), messageWithNick.size(), 0) < 0)
		{
			std::cerr << "send() failed" << std::endl;
			return ;
		}
	}
	else
	{
		client.sendMessage(ERR_UNKNOWNCOMMAND(msj.args[0]));
		return ;
	}
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
	if (handle_authentification(client, msj, server))
		return ;
	if (client.getIs_auth() == false)
		return ;
	if (toUpper(msj.args[0]) == "JOIN")
		handle_join(server, client, msj);
	else if (toUpper(msj.args[0]) == "INVITE")
		handle_invite(server, client, msj);
	else if (toUpper(msj.args[0]) == "TOPIC")
		handle_topic(server, client, msj);
	else if (toUpper(msj.args[0]) == "PRIVMSG")
		handle_privmsg(server, client, msj);
	else if (toUpper(msj.args[0]) == "MODE")
		handleChannelMode(client, msj, server);
	else if (toUpper(msj.args[0]) == "KICK")
		handleKickCommand(client, msj, server);
	else if (msj.args[0].find("!") != std::string::npos)
		parse_bot_command(msj, client, server);
	else if (!msj.args[0].empty())
		client.sendMessage(ERR_UNKNOWNCOMMAND(msj.args[0]));
}
