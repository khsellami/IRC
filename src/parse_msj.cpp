
#include "../include/header.hpp"

std::string getCommand(const std::string &msg)
{
	return msg.substr(0, msg.find(" "));
}

std::string getArgument(const std::string &msg)
{
	size_t pos = msg.find(" ");
	return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
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

void parse_message(const std::string &msg1, Client &client, const char* password, std::map<int , Client> clients, Server &server, std::map<std::string, Channel> channels)
{
	(void)channels;
	Msj msj;
	std::string msg = trim(msg1);
	std::stringstream  ss(msg);
	std::string word;
	while (ss >> word)
		msj.args.push_back(word);
	msj.orig_msg = msg;
	handle_authentification(client, std::string(password), msj, clients, server);
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

	//////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	To handle 2 cases:
		privmsg target :message 
		:message privmsg target
	*/
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "TOPIC")
	{
		handle_topic(server, client, msj);
	}
	else if ((msj.args.size() > 0 && toUpper(msj.args[0]) == "PRIVMSG") || (msj.args.size() > 1 && toUpper(msj.args[1]) == "PRIVMSG"))
	{
		handle_privmsg(server, client, msj);
	}
	else if (msj.args.size() > 0 && toUpper(msj.args[0]) == "MODE")
	{
		handleChannelMode(client, msj, server);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
}
