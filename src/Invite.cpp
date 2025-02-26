#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector> 



void handle_invite(Server &server, Client &client, Msj &msj)
{
    if (msj.args.size() < 3) // Vérifie que <nickname> et <channel> sont fournis
    {
        client.sendMessage("461 INVITE :Not enough parameters");
        return;
    }

    std::string target_nickname = msj.args[1]; // Le client à inviter
    std::string channel_name = msj.args[2];    // Le canal

    // Vérifier si le canal existe
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator it;
    for(it = channels.begin(); it != channels.end(); it++)
    {
        std::cout << "0000000000000000000000000\n";
        std::cout << "Channel: " << it->first << std::endl;
    }
    if (channels.find(channel_name) == channels.end())
    {
        client.sendMessage("403 " + channel_name + " :No such channel");
        return;
    }
    Channel &channel = channels[channel_name];

    // Vérifier si l'émetteur est membre du canal
    if (!channel.isMember(client))
    {
        client.sendMessage("442 " + channel_name + " :You're not on that channel");
        return;
    }

    // Vérifier si l'utilisateur à inviter existe
    Client *target_client = server.getClientByName(target_nickname);
    if (!target_client)
    {
        client.sendMessage("401 " + target_nickname + " :No such nick");
        return;
    }

    // Vérifier si le canal est en mode +i et si l'émetteur est opérateur
    if (channel.iSInviteOnly() && !channel.isOperator(client))
    {
        client.sendMessage("482 " + channel_name + " :You're not channel operator");
        return;
    }

    // Vérifier si la cible est déjà membre du canal
    if (channel.isMember(*target_client))
    {
        client.sendMessage("443 " + target_nickname + " " + channel_name + " :is already on channel");
        return;
    }

    // Ajouter la cible à la liste des invités
    channel.invite(*target_client);
    target_client->sendMessage("341 " + client.getNickName() + " " + target_nickname + " " + channel_name);

    std::cout << "[INVITE] " << client.getNickName() << " invited " << target_nickname << " to " << channel_name << std::endl;
}


// void handle_invite(Server &server, Client &client, Msj &msj)
// {
//     if (msj.args.size() < 3)
//     {
//         client.sendMessage("461 INVITE :Not enough parameters"); // ERR_NEEDMOREPARAMS
//         return;
//     }

//     std::string target_nick = msj.args[1];
//     std::string channel_name = msj.args[2];

//     Client *target_client = server.getClientByName(target_nick);
//     if (!target_client)
//     {
//         client.sendMessage("401 " + target_nick + " :No such nick"); // ERR_NOSUCHNICK
//         return;
//     }

//     std::map<std::string, Channel> &channels = server.getChannels();
//     if (channels.find(channel_name) == channels.end())
//     {
//         client.sendMessage("403 " + channel_name + " :No such channel"); // ERR_NOSUCHCHANNEL
//         return;
//     }

//     Channel &channel = channels[channel_name];

//     if (!channel.isMember(client))
//     {
//         client.sendMessage("442 " + channel_name + " :You're not on that channel"); // ERR_NOTONCHANNEL
//         return;
//     }

//     if (channel.isMember(*target_client))
//     {
//         client.sendMessage("443 " + target_nick + " " + channel_name + " :is already on channel"); // ERR_USERONCHANNEL
//         return;
//     }

//     if (channel.iSInviteOnly() && !channel.isOperator(client))
//     {
//         client.sendMessage("482 " + channel_name + " :You're not a channel operator"); // ERR_CHANOPRIVSNEEDED
//         return;
//     }

//     channel.invite(*target_client);

//     client.sendMessage("341 " + target_nick + " " + channel_name); // RPL_INVITING
//     target_client->sendMessage(":" + client.getName() + " INVITE " + target_nick + " " + channel_name);
// }

