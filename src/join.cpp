#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector> // Add this line

// void handle_join(Client &client, Msj &msj)
// {
//     Channel newChannel;
//     Server server;
//     std::string channel_name = msj.getArgs()[0];
//     std::cout << "Channel name: " << channel_name << '\n';
//     if (server.getChannels().find(channel_name) == server.getChannels().end())
//     {
//         newChannel.getName() = channel_name;
//         server.getChannels()[channel_name] = newChannel;
//         std::cout << "Channel created: " << channel_name << '\n';
//     }
//     server.getChannels()[channel_name].getMembers().push_back(client);
//     std::cout << "Client joined channel: " << channel_name << '\n';
// }
void handle_join(Server &server, Client &client, Msj &msj)
{
    if (msj.getArgs().empty()) {
        std::cerr << "Error: No channel name provided!\n";
        return;
    }

    std::string raw_channel_name = msj.getArgs()[0];

    // Validate that the channel name starts with '#'
    if (raw_channel_name.empty() || raw_channel_name[0] != '#') {
        std::cerr << "Error: Invalid channel name! Must start with '#'.\n";
        return;
    }

    // Extract only the part after '#'
    std::string channel_name = raw_channel_name.substr(0); 

    if (channel_name.empty()) {
        std::cerr << "Error: Channel name cannot be empty after '#'.\n";
        return;
    }

    std::cout << "Channel name: " << channel_name << '\n';

    // Check if the channel exists, if not, create it
    if (server.getChannels().find(channel_name) == server.getChannels().end()) {
        Channel newChannel;
        newChannel.setName(channel_name);
        server.addChannel(channel_name, newChannel);
        std::cout << "Channel created: " << channel_name << '\n';
    }

    // Add client to the channel
    server.getChannels()[channel_name].addMember(client);
    std::cout << "Client joined channel: " << channel_name << '\n';
}


