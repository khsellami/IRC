#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector> 



void handle_invite(Server &server, Client &client, Msj &msj)
{
    if (msj.args.size() < 3)
    {
        client.sendMessage("461 INVITE :Not enough parameters"); // ERR_NEEDMOREPARAMS
        return;
    }

    std::string target_nick = msj.args[1];
    std::string channel_name = msj.args[2];

    Client *target_client = server.getClientByName(target_nick);
    if (!target_client)
    {
        client.sendMessage("401 " + target_nick + " :No such nick"); // ERR_NOSUCHNICK
        return;
    }

    std::map<std::string, Channel> &channels = server.getChannels();
    if (channels.find(channel_name) == channels.end())
    {
        client.sendMessage("403 " + channel_name + " :No such channel"); // ERR_NOSUCHCHANNEL
        return;
    }

    Channel &channel = channels[channel_name];

    if (!channel.isMember(client))
    {
        client.sendMessage("442 " + channel_name + " :You're not on that channel"); // ERR_NOTONCHANNEL
        return;
    }

    if (channel.isMember(*target_client))
    {
        client.sendMessage("443 " + target_nick + " " + channel_name + " :is already on channel"); // ERR_USERONCHANNEL
        return;
    }

    if (channel.isInviteOnly() && !channel.isOperator(client))
    {
        client.sendMessage("482 " + channel_name + " :You're not a channel operator"); // ERR_CHANOPRIVSNEEDED
        return;
    }

    channel.invite(*target_client);

    client.sendMessage("341 " + target_nick + " " + channel_name); // RPL_INVITING
    target_client->sendMessage(":" + client.getName() + " INVITE " + target_nick + " " + channel_name);
}
