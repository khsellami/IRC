#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector>

// it swrks

// void handle_join(Server &server, Client &client, Msj &msj)
// {
//     for (size_t i = 0; i < msj.args.size(); i++)
//     {
//         std::cout << "------------------>" << msj.args[i] << std::endl;
//     }
//     if (msj.args.size() < 2)
//     {
//         client.sendMessage("431 You have to provide a channel name.");
//         return;
//     }

//     std::string channel_name = msj.args[1];

//     // Get reference to server channels
//     std::map<std::string, Channel> &channels = server.getChannels();

//     // Check if the channel exists
//     if (channels.find(channel_name) == channels.end())
//     {
//         // If not, create a new channel and add it to the server
//         std::cout << "Creating new channel: " << channel_name << std::endl;
//         Channel new_channel(channel_name);
//         server.addChannel(channel_name, new_channel);
//     }
//     Channel &channel = channels[channel_name];
//     if (!channel.isMember(client))
//     {
//         channel.addMember(client);
//         std::cout << "Client joined channel: " << channel.getName() << std::endl;
//     }
//     else
//     {
//         client.sendMessage("You are already a member of this channel.");
//     }
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isJoinAllowed(Client &client, Channel &channel, const std::string &key)
{
    std::cout << channel.isInvited(client) << std::endl;
    if (channel.iSInviteOnly() && !channel.isInvited(client))
    {
        client.sendMessage("473 " + channel.getName() + " :Cannot join channel (+i) - you must be invited"); // ERR_INVITEONLYCHAN
        return false;
    }

    if (channel.isBanned(client))
    {
        client.sendMessage("474 " + channel.getName() + " :Cannot join channel - you are banned"); // ERR_BANNEDFROMCHAN
        return false;
    }

    if (channel.hasKey() && channel.getKey() != key)
    {
        client.sendMessage("475 " + channel.getName() + " :Cannot join channel - incorrect key"); // ERR_BADCHANNELKEY
        return false;
    }

    if (channel.isMember(client))
    {
        client.sendMessage("462 " + client.getName() + " :You are already in this channel.");
        return false;
    }
    if (channel.isFull())
    {
        client.sendMessage("471 " + channel.getName() + " :Cannot join channel - channel is full"); // ERR_CHANNELISFULL
        return false;
    }

    return true;
}


void sendJoinReplies(Client &client, Channel &channel)
{
    if (!channel.getTopic().empty())
    {
        client.sendMessage("332 " + client.getName() + " " + channel.getName() + " :" + channel.getTopic()); // RPL_TOPIC
    }
    client.sendMessage("353 " + client.getName() + " = " + channel.getName() + " :" + channel.getUserList()); // RPL_NAMREPLY
    client.sendMessage("366 " + client.getName() + " " + channel.getName() + " :End of /NAMES list"); // RPL_ENDOFNAMES
}
bool only_one_channel(const std::vector<std::string> &args)
{
    if (args.empty()) 
        return true;

    std::string channels = args[1];

    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i] == ',' && i + 1 < channels.size() && channels[i + 1] == '#')
            return false;
    }
    
    return true;
}
 std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}
void handle_join(Server &server, Client &client, Msj &msj)
{
    if (msj.args[1].empty())
    {
        client.sendMessage("461 JOIN :ERR_NEEDMOREPARAMS");
        return;
    }

    std::vector<std::string> channels = split(msj.args[1], ',');
    std::vector<std::string> keys;
    
    if (msj.args.size() > 2)
        keys = split(msj.args[2], ',');
    
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string channel_name = channels[i];
        if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
        {
            client.sendMessage("403 " + channel_name + " :ERR_NOSUCHCHANNEL");
            continue;
        }
        channel_name = channel_name.substr(1);

        std::map<std::string, Channel> &channelMap = server.getChannels();
        bool is_new_channel = (channelMap.find(channel_name) == channelMap.end());

        if (is_new_channel)
        {
            std::cout << "Creating new channel: " << channel_name << std::endl;
            server.addChannel(channel_name, Channel(channel_name));
        }

        Channel &channel = channelMap[channel_name];
        std::string key = (i < keys.size()) ? keys[i] : "";

        if (!isJoinAllowed(client, channel, key))
            continue;

        channel.addMember(client);
        
        if (is_new_channel)
        {
            channel.setOperator(client);
            std::cout << client.getName() << " is now the operator of channel: " << channel.getName() << std::endl;
        }

        std::cout << client.getName() << " joined channel: " << channel.getName() << std::endl;
        channel.broadcast(client.getPrefix() + " JOIN #" + channel_name);
        sendJoinReplies(client, channel);
    }
}
