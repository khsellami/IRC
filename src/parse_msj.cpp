#include "../include/Msj.hpp"
#include "../include/Client.hpp"
#include <iostream>
#include "../include/Server.hpp"
#include <algorithm>



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
	(void)password;
	Msj msj;
	std::string msg = trim(msg1);
	std::cout << "The message is: " << msg << '\n';
	std::string command = getCommand(msg);
	std::string argument = getArgument(msg);
	std::string CMD = toUpper(command);
	std::stringstream  ss(msg);
	std::string word;
	while(ss >> word)
		msj.args.push_back(word);
	handle_authentification(client, std::string(password), msj, clients);
	if (CMD == "TOPIC")
	{
		handle_topic(server, client,clients, msj, msg, channels);
	}
	else if (CMD == "JOIN")
	{
        handle_join(server, client, msj);
	}
	else if  (CMD == "INVITE")
    {
        handle_invite(server, client, msj);
    }
}
