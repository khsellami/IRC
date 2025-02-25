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

    std::string channel_name = msj.getArgs()[0];
    std::cout << "Channel name: " << channel_name << '\n';

    if (server.getChannels().find(channel_name) == server.getChannels().end()) {
        // Create a new channel and add it to the server
        Channel newChannel;
        newChannel.setName(channel_name); // Use a setter
        server.getChannels()[channel_name] = newChannel;
        std::cout << "Channel created: " << channel_name << '\n';
    }

    // Add client to the channel
    server.getChannels()[channel_name].addMember(client); // Use a proper addMember function
    std::cout << "Client joined channel: " << channel_name << '\n';
}
