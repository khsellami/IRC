#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector> 



void handle_invite(Server &server, Client &client, Msj &msj)
{
    if (msj.args[1].size() < 2) { 
        std::cerr << "Error: Channel name and client to invite required!\n";
        return;
    }

    std::string channel_name = msj.args[1];
    std::string target_client_name = msj.args[2];

    // Validate that the channel name starts with '#'
    // if (channel_name.empty() || channel_name[0] != '#') {
    //     std::cerr << "Error: Invalid channel name! Must start with '#'.\n";
    //     return;
    // }

    // Extract the actual channel name without '#'
    channel_name = channel_name.substr(1);

    std::cout << "Channel name: " << channel_name << '\n';

    // Check if the client is trying to invite themselves
    if (client.getName() == target_client_name) {
        std::cerr << "Error: You cannot invite yourself to a channel!\n";
        return;
    }

    // Check if the channel exists
    if (server.getChannels().find(channel_name) == server.getChannels().end()) {
        std::cerr << "Error: Channel does not exist!\n";
        return;
    }

    // Check if the inviting client is a member of the channel
    if (!server.getChannels()[channel_name].isMember(client)) {
        std::cerr << "Error: You must be a member of the channel to invite others!\n";
        return;
    }

    // Find the target client
    Client *target_client = server.getClientByName(target_client_name);
    if (!target_client) {
        std::cerr << "Error: Target client does not exist!\n";
        return;
    }

    // Invite the target client
    server.getChannels()[channel_name].invite(*target_client);
    std::cout << "Client " << target_client_name << " invited to channel: " << channel_name << '\n';
}
