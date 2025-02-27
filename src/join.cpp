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
void handle_join(Server &server, Client &client, Msj &msj)
{
    if (msj.args.empty())
    {
        client.sendMessage("461 JOIN :Not enough parameters"); // ERR_NEEDMOREPARAMS
        return;
    }

    std::string channel_name = msj.args[1]; // First parameter is the channel name
    if (!channel_name.empty() && channel_name[1] == '#') 
    {
        channel_name = channel_name.substr(1);
    }
    std::string key = (msj.args.size() > 1) ? msj.args[1] : ""; // Second parameter is optional key

    std::map<std::string, Channel> &channels = server.getChannels();

    bool is_new_channel = (channels.find(channel_name) == channels.end());

    // Create the channel if it does not exist
    if (is_new_channel)
    {
        std::cout << "Creating new channel: " << channel_name << std::endl;
        Channel new_channel(channel_name);
        server.addChannel(channel_name, new_channel);
    }

    Channel &channel = channels[channel_name];

    // === INVITE-ONLY CHECK ===
    if (channel.iSInviteOnly() && !channel.isInvited(client))
    {
        client.sendMessage("473 " + channel_name + " :Cannot join channel (+i) - you must be invited"); // ERR_INVITEONLYCHAN
        return;
    }

    // === CHECK IF CLIENT IS BANNED ===
    if (channel.isBanned(client))
    {
        client.sendMessage("474 " + channel_name + " :Cannot join channel - you are banned"); // ERR_BANNEDFROMCHAN
        return;
    }

    // === CHECK IF THE CHANNEL HAS A PASSWORD (KEY) ===
    if (channel.hasKey() && channel.getKey() != key)
    {
        client.sendMessage("475 " + channel_name + " :Cannot join channel - incorrect key"); // ERR_BADCHANNELKEY
        return;
    }

    // === ADD CLIENT TO CHANNEL ===
    if (!channel.isMember(client))
    {
        channel.addMember(client);
        
        // If this is a new channel, make the first client the operator
        if (is_new_channel)
        {
            client.setOperator(true);
            channel.setOperator(client);
            std::cout << client.getName() << " is now the operator of channel: " << channel.getName() << std::endl;
            std::cout <<"!!!!!!!!!!!!!!!!!"<< client.getIs_operator() << std::endl;
        }

        std::cout << client.getName() << " joined channel: " << channel.getName() << std::endl;

        // Send JOIN confirmation to all members
        channel.broadcast(client.getPrefix() + " JOIN " + channel_name);

        // === SEND TOPIC IF EXISTS ===
        if (!channel.getTopic().empty())
        {
            client.sendMessage("332 " + client.getName() + " " + channel_name + " :" + channel.getTopic()); // RPL_TOPIC
        }

        // === SEND LIST OF USERS ===
        std::string user_list = channel.getUserList();
        client.sendMessage("353 " + client.getName() + " = " + channel_name + " :" + user_list); // RPL_NAMREPLY
        client.sendMessage("366 " + client.getName() + " " + channel_name + " :End of /NAMES list"); // RPL_ENDOFNAMES
    }
    else
    {
        client.sendMessage("462 " + client.getName() + " :You are already in this channel.");
    }
}

