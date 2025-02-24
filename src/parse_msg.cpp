
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"


void handle_join(Client &client, std::string argument)
{
	(void)client;
	(void)argument;
	std::cout << "Join command" << '\n';
}
std::string getCommand(const std::string &msg)
{
    return msg.substr(0, msg.find(" "));
}

std::string getArgument(const std::string &msg)
{
    size_t pos = msg.find(" ");
    return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
}

#include <algorithm>
#include <sstream>
std::string toUpper(const std::string &str)
{
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}
std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

void parse_message(const std::string &msg1, Client &client, std::map<int , Client> clients, std::map<std::string, Channel> channels)
{
	(void)client;
	(void)clients;
	(void)channels;

	std::cout << "Enter here with the message : " << msg1 << '\n';
	Msj msj;
	std::string msg = trim(msg1);
    // std::cout << "The message is: ----->" << msg << '\n';
    
    std::string command;
	std::istringstream stream(msg);
	std::getline(stream, command, ' ');
	std::string CMD = toUpper(command);
	msj.setCommand(CMD);
	std::string arg;
	while (std::getline(stream >> std::ws, arg, ' '))
	{
		arg.erase(std::remove(arg.begin(), arg.end(), '\n'), arg.end()); 
		msj.args.push_back(arg);
	}
	std::cout << "Command is ------>" << msj.getCommand() << '\n';
	std::cout << "Arguments: ------>";
	for (size_t i = 0; i < msj.args.size(); ++i) {
		std::cout << "[" << msj.args[i] << "]";
	}
	std::cout << '\n';
	// if(CMD == "JOIN")
	// 	handle_join(client, argument);
	// if(CMD == "NICK")
	// 	handle_nick(client, argument);
	// if(CMD == "USER")
	// 	handle_user(client, argument);
	// if(CMD == "PRIVMSG")
	// 	handle_privmsg(client, argument);
	// if(CMD == "QUIT")
	// 	handle_quit(client, argument);
	// if(CMD == "TOPIC")
	// 	handle_topic(client, argument);
	// if(CMD == "INVITE")
	// 	handle_invite(client, argument);
	
}