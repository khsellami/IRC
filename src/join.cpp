#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector>



void handle_join(Server &server, Client &client, Msj &msj)
{
    for (size_t i = 0; i < msj.args.size(); i++)
    {
        std::cout << "------------------>" << msj.args[i] << std::endl;
    }
    if (msj.args.size() < 2)
    {
        client.sendMessage("431 You have to provide a channel name.");
        return;
    }

    std::string channel_name = msj.args[1];

    // Get reference to server channels
    std::map<std::string, Channel> &channels = server.getChannels();

    // Check if the channel exists
    if (channels.find(channel_name) == channels.end())
    {
        // If not, create a new channel and add it to the server
        std::cout << "Creating new channel: " << channel_name << std::endl;
        Channel new_channel(channel_name);
        server.addChannel(channel_name, new_channel);
    }
    Channel &channel = channels[channel_name];
    if (!channel.isMember(client))
    {
        channel.addMember(client);
        std::cout << "Client joined channel: " << channel.getName() << std::endl;
    }
    else
    {
        client.sendMessage("You are already a member of this channel.");
    }
}

