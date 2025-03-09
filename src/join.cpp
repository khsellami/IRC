#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector>
#include "../include/Reply.hpp"

bool isJoinAllowed(Client &client, Channel &channel, const std::string &key)
{
    if (channel.iSInviteOnly() && !channel.isInvited(client))
    {
        client.sendMessage(ERR_INVITEONLYCHAN(client.getName(), channel.getName()));
        return false;
    }

    if (channel.isBanned(client))
    {
        client.sendMessage(ERR_BANNEDFROMCHAN(channel.getName(), client.getName()));
        return false;
    }

    if (channel.hasKey() && channel.getKey() != key)
    {
        client.sendMessage(ERR_BADCHANNELKEY(channel.getName(), client.getName()));
        return false;
    }

    if (channel.isMember(client))
    {
        client.sendMessage(ERR_ALREADYREGISTRED(client.getName()));
        return false;
    }

    if (channel.isFull())
    {
        client.sendMessage(ERR_CHANNELISFULL(client.getName(), channel.getName()));
        return false;
    }

    return true;
}



void sendJoinReplies(Client &client, Channel &channel)
{
    if (!channel.getTopic().empty())
    {
        client.sendMessage(RPL_TOPIC(client.getName(), channel.getName(), channel.getTopic()));
    }
    client.sendMessage(RPL_NAMREPLY(client.getName(), channel.getName(), channel.getUserList()));
    client.sendMessage(RPL_ENDOFNAMES(client.getName(), channel.getName()));
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
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getName()));
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
            client.sendMessage(ERR_NOSUCHCHANNEL(channel_name));
            continue;
        }
        channel_name = channel_name.substr(1);

        std::map<std::string, Channel> &channelMap = server.getChannels();
        bool is_new_channel = (channelMap.find(channel_name) == channelMap.end());

        if (is_new_channel)
            server.addChannel(channel_name, Channel(channel_name));

        Channel &channel = channelMap[channel_name];
        std::string key = (i < keys.size()) ? keys[i] : "";

        if (!isJoinAllowed(client, channel, key))
            continue;

        channel.addMember(client);
        
        if (is_new_channel)
        {
            channel.setOperator(client);
        }
        channel.broadcast(client.getPrefix() + " JOIN #" + channel_name);
        sendJoinReplies(client, channel);
    }
}
