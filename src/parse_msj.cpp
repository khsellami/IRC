#include "../include/Msj.hpp"
#include "../include/Client.hpp"
#include <iostream>
#include "../include/Server.hpp"



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
std::string geting_message(const std::string &msg)
{
    size_t pos = msg.find(":");
    return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
}

void parse_message(const std::string &msg1, Client &client)
{
    Msj msj;
    (void)client;
    std::string msg = trim(msg1);
    std::cout << "The message is: " << msg << '\n';

    std::string command = getCommand(msg);
    std::string argument = getArgument(msg);

    std::string CMD = toUpper(command);
    msj.setCommand(CMD);



    msj.setArgument(argument);
    msj.SetArgs(argument);
    for (size_t i = 0; i < msj.getArgs().size(); i++)
    {
        std::cout << "[" << msj.getArgs()[i] << "]\n";
    }
    // std::cout << "Message: " << msj.get_message() << '\n';

    if (CMD == "JOIN")
    {
        std::cout << "Handling JOIN command...\n";
        handle_join(client, msj);
    }
}