#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector> 


bool checkChannelExist(std::map<std::string, Channel> &channels, std::string channel_name)
{
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->first == channel_name)
            return true;
    }
    return false;
}

void handle_invite(Server &server, Client &client, Msj &msj)
{
    if (msj.args.size() < 3)
    {
        client.sendMessage("461 INVITE :// ERR_NEEDMOREPARAMS");
        return;
    }

    std::string target_nickname = msj.args[1]; // The client to invite

    std::string channel_name =  (msj.args[2][0] == '#' || msj.args[2][0] == '&') ? msj.args[2].substr(1) : msj.args[2];

    // Verify if the target user exists
    Client *target_client = server.getClientByName(target_nickname);
    if (!target_client)
    {
        client.sendMessage("401 " + target_nickname + " :// ERR_NOSUCHNICK");
        return;
    }
    if (checkChannelExist(server.getChannels(), channel_name) == false)
    {
        client.sendMessage("341 :" + client.getNickName() + " " + target_nickname + " " + channel_name);
        target_client->sendMessage(":" + client.getNickName() + " INVITE " + target_nickname + " " + channel_name);
        std::cout << "[INVITE] " << client.getNickName() << " invited " << target_nickname << " to " << channel_name << " (channel does not exist yet)." << std::endl;
        return;
    }

    Channel &channel = server.getChannels()[channel_name];
    if (!channel.isMember(client))
    {
        client.sendMessage("442 " + channel_name + " :ERR_NOTONCHANNEL");
        return;
    }
    if (channel.iSInviteOnly() && !channel.isOperator(client))
    {
        client.sendMessage("482 " + channel_name + " :ERR_CHANOPRIVSNEEDED");
        return;
    }
    if (channel.isMember(*target_client))
    {
        client.sendMessage("443 " + target_nickname + " " + channel_name + " :ERR_USERONCHANNEL");
        return;
    }
    channel.invite(*target_client);
    client.sendMessage("341 " + client.getNickName() + " " + target_nickname + " " + channel_name); // RPL_INVITING
    target_client->sendMessage(":" + client.getNickName() + " INVITE " + target_nickname + " " + channel_name);
    std::cout << "[INVITE] " << client.getNickName() << " invited " << target_nickname << " to " << channel_name << std::endl;
}
