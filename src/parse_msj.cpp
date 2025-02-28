
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
}
