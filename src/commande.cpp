#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <stdlib.h>
#include "msj.hpp"
#include <iostream>
#include <sstream>   
std::map<int, User> clients;  // Associe un socket à un utilisateur
std::map<std::string, std::set<int > > channels;  // Associe un channel à une liste de sockets
std::map<std::string, std::set<int> > channel_operators; // Channel operators
std::map<std::string, std::set<char> > channel_modes;   // Active modes (+i, +t, etc.)
std::map<std::string, std::string> channel_passwords;  // Passwords for channels
std::map<std::string, std::string> channel_topics;    // Topic for each channel
std::map<std::string, int> channel_user_limits;  // User limits
void handle_nick(int client_fd, std::istringstream &iss)
{
    std::string nickname;
    iss >> nickname;
    if (nickname.empty())
    {
        send(client_fd, "Error: NICK command requires a nickname\n", 40, 0);
        return;
    }
    clients[client_fd].setNickname(nickname);
    send(client_fd, "Nickname set successfully\n", 26, 0);
}

void handle_user(int client_fd, std::istringstream &iss)
{
    std::string username;
    iss >> username;
    if (username.empty())
    {
        send(client_fd, "Error: USER command requires a username\n", 40, 0);
        return;
    }
    clients[client_fd].setUsername(username);
    send(client_fd, "Username set successfully\n", 26, 0);
}

void handle_privmsg(int client_fd, std::istringstream &iss)
{
    std::string target, message;
    iss >> target;
    std::getline(iss, message);
    message = message.substr(1);

    if (target.empty() || message.empty())
    {
        send(client_fd, "Error: PRIVMSG requires a target and a message\n", 47, 0);
        return;
    }

    if (target[0] == '#') 
    {
        for (std::set<int>::iterator it = channels[target].begin(); it != channels[target].end(); ++it)
        {
            if (*it != client_fd)
                send(*it, (clients[client_fd].getNickname() + ": " + message + "\n").c_str(), 
                    message.size() + clients[client_fd].getNickname().size() + 3, 0);
        }


    }
    else 
    {
        for (std::map<int, User>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->second.getNickname() == target)
            {
                send(it->first, (clients[client_fd].getNickname() + " (private): " + message + "\n").c_str(), 
                    message.size() + clients[client_fd].getNickname().size() + 11, 0);
                return;
            }
        }


        send(client_fd, "Error: User not found\n", 22, 0);
    }
}

void handle_kick(int client_fd, std::istringstream &iss)
{
    std::string channel, user;
    iss >> channel >> user;
    if (channel.empty() || user.empty())
    {
        send(client_fd, "Error: KICK requires a channel and a user\n", 42, 0);
        return;
    }
    
        for (std::set<int>::iterator it = channels[channel].begin(); it != channels[channel].end(); ++it)
        {
            if (clients[*it].getNickname() == user)
            {
                std::string kickMsg = "You have been kicked from " + channel + "\n";
                send(*it, kickMsg.c_str(), kickMsg.size(), 0);
                channels[channel].erase(it);
                return;
            }
        }

    send(client_fd, "Error: User not found in channel\n", 33, 0);
}


void handle_join(int client_fd, std::istringstream &iss)
{
    std::string channel, password;
    iss >> channel >> password;

    if (channel.empty())
    {
        send(client_fd, "Error: JOIN requires a channel name\n", 36, 0);
        return;
    }

    // Check if the channel exists
    if (channels.find(channel) == channels.end())
    {
        // Create the channel if it doesn't exist
        channels[channel].insert(client_fd);
        channel_operators[channel].insert(client_fd); // First user is the operator
        send(client_fd, ("You have created and joined " + channel + "\n").c_str(), 
             channel.size() + 27, 0);
    }
    else
    {
        // Check if the channel is invite-only
        if (channel_modes[channel].count('i') > 0 && channel_operators[channel].count(client_fd) == 0)
        {
            send(client_fd, "Error: This channel is invite-only\n", 35, 0);
            return;
        }

        // Check if the channel has a password
        if (channel_passwords.find(channel) != channel_passwords.end())
        {
            if (channel_passwords[channel] != password)
            {
                send(client_fd, "Error: Incorrect channel password\n", 34, 0);
                return;
            }
        }

        // Check if the channel has a user limit
        if (channel_user_limits.find(channel) != channel_user_limits.end() &&
            static_cast<int>(channels[channel].size()) >= channel_user_limits[channel])
        {
            send(client_fd, "Error: Channel is full\n", 23, 0);
            return;
        }

        // Add the user to the channel
        channels[channel].insert(client_fd);
        send(client_fd, ("You have joined " + channel + "\n").c_str(), 
             channel.size() + 17, 0);
    }

    // Notify other users in the channel
    std::string join_message = clients[client_fd].getNickname() + " has joined " + channel + "\n";
    for (std::set<int>::iterator it = channels[channel].begin(); it != channels[channel].end(); ++it)
    {
        if (*it != client_fd)
        {
            send(*it, join_message.c_str(), join_message.size(), 0);
        }
    }
}
void handle_invite(int client_fd, std::istringstream &iss)
{
    std::string username, channel;
    iss >> username >> channel;

    if (username.empty() || channel.empty())
    {
        send(client_fd, "Error: INVITE requires a username and a channel\n", 48, 0);
        return;
    }

    // Check if the channel exists
    if (channels.find(channel) == channels.end())
    {
        send(client_fd, "Error: Channel does not exist\n", 30, 0);
        return;
    }

    // Check if the user is an operator in the channel
    if (channel_operators[channel].count(client_fd) == 0)
    {
        send(client_fd, "Error: Only operators can invite users\n", 40, 0);
        return;
    }

    // Find the target user by nickname
    int target_fd = -1;
    for (std::map<int, User>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.getNickname() == username)
        {
            target_fd = it->first;
            break;
        }
    }

    if (target_fd == -1)
    {
        send(client_fd, "Error: User not found\n", 22, 0);
        return;
    }

    send(target_fd, ("You have been invited to join " + channel + "\n").c_str(), 
         channel.size() + 30, 0);
    send(client_fd, ("User " + username + " has been invited to " + channel + "\n").c_str(), 
         username.size() + channel.size() + 31, 0);
}

void handle_topic(int client_fd, std::istringstream &iss)
{
    std::string channel, topic;
    iss >> channel;
    std::getline(iss, topic);
    topic = topic.substr(1); // Remove leading space

    if (channel.empty())
    {
        send(client_fd, "Error: TOPIC requires a channel name\n", 37, 0);
        return;
    }

    // Check if the channel exists
    if (channels.find(channel) == channels.end())
    {
        send(client_fd, "Error: Channel does not exist\n", 30, 0);
        return;
    }

    // If no topic is provided, return the current topic
    if (topic.empty())
    {
        if (channel_topics.find(channel) == channel_topics.end() || channel_topics[channel].empty())
            send(client_fd, "No topic is set for this channel\n", 33, 0);
        else
            send(client_fd, ("Topic for " + channel + ": " + channel_topics[channel] + "\n").c_str(), 
                 channel.size() + channel_topics[channel].size() + 10, 0);
        return;
    }

    // Check if topic change requires operator privileges (`+t` mode)
    if (channel_modes[channel].count('t') > 0 && channel_operators[channel].count(client_fd) == 0)
    {
        send(client_fd, "Error: Only operators can change the topic\n", 44, 0);
        return;
    }

    // Set the new topic
    channel_topics[channel] = topic;
    send(client_fd, "Topic updated successfully\n", 27, 0);

    // Notify all users in the channel
    std::string topic_message = "New topic for " + channel + ": " + topic + "\n";
    for (std::set<int>::iterator it = channels[channel].begin(); it != channels[channel].end(); ++it)
    {
        send(*it, topic_message.c_str(), topic_message.size(), 0);
    }
}
void handle_mode(int client_fd, std::istringstream &iss)
{
    std::string channel, mode, param;
    iss >> channel >> mode >> param;

    if (channel.empty() || mode.empty())
    {
        send(client_fd, "Error: MODE requires a channel and a mode\n", 42, 0);
        return;
    }

    if (channels.find(channel) == channels.end())
    {
        send(client_fd, "Error: Channel does not exist\n", 30, 0);
        return;
    }

    if (channel_operators[channel].count(client_fd) == 0)
    {
        send(client_fd, "Error: Only operators can change channel modes\n", 47, 0);
        return;
    }

    char sign = mode[0];
    char mode_flag = mode[1];

    if (sign != '+' && sign != '-')
    {
        send(client_fd, "Error: Mode must start with '+' or '-'\n", 39, 0);
        return;
    }

    int target_fd = -1;

    switch (mode_flag)
    {
        case 'i': // Invite-only mode
            if (sign == '+') channel_modes[channel].insert('i');
            else channel_modes[channel].erase('i');
            break;

        case 't': // Topic restriction mode
            if (sign == '+') channel_modes[channel].insert('t');
            else channel_modes[channel].erase('t');
            break;

        case 'k': // Password-protected channel
            if (sign == '+')
            {
                if (param.empty())
                {
                    send(client_fd, "Error: MODE +k requires a password\n", 36, 0);
                    return;
                }
                channel_passwords[channel] = param;
            }
            else
                channel_passwords.erase(channel);
            break;

        case 'o': // Operator mode
            if (param.empty())
            {
                send(client_fd, "Error: MODE +o requires a username\n", 36, 0);
                return;
            }

            for (std::map<int, User>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->second.getNickname() == param)
                {
                    target_fd = it->first;
                    break;
                }
            }

            if (target_fd == -1)
            {
                send(client_fd, "Error: User not found\n", 22, 0);
                return;
            }
            if (sign == '+') channel_operators[channel].insert(target_fd);
            else channel_operators[channel].erase(target_fd);
            break;

        case 'l': // User limit mode
            if (sign == '+')
            {
                std::istringstream iss(param);
                int limit;
                iss >> limit;
                if (iss.fail() || limit <= 0)
                {
                    send(client_fd, "Error: MODE +l requires a positive number\n", 42, 0);
                    return;
                }
                channel_user_limits[channel] = limit;
            }
            else
                channel_user_limits.erase(channel);
            break;

        default:
            send(client_fd, "Error: Invalid mode\n", 20, 0);
            return;
    }

    send(client_fd, "Mode updated successfully\n", 26, 0);
}
