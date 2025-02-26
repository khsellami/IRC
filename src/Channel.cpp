#include "../include/Channel.hpp"

Channel::Channel(){}

Channel::Channel(std::string name) : name_channel(name) ,inviteOnly(false) {}

std::string  Channel::getName() { return name_channel; }

bool Channel::hasKey() { return !key.empty(); }

bool Channel::iSInviteOnly() { return inviteOnly; }

void Channel::setKey(std::string newKey) { key = newKey; }
void Channel::setInviteOnly(bool status) { inviteOnly = status; }
void Channel::setName(std::string name) { name_channel = name; }
bool Channel::isBanned(Client &client)
{
    return std::find(bannedUsers.begin(), bannedUsers.end(), client.getName()) != bannedUsers.end();
}
// bool Channel::isInvited(Client &client)
// {
//     return std::find(invited.begin(), invited.end(), client) != invited.end();
// }
bool Channel::getT(){return t;}
std::string Channel::getUserList()
{
    std::string list;
    for (size_t i = 0; i < members.size(); i++)
    {
        list += members[i].getName() + " ";
    }
    return list;
}
void Channel::setTopic(std::string topic){this->topic = topic;}
void Channel::broadcast(const std::string &message)
{
    for (size_t i = 0; i < members.size(); i++)
    {
        members[i].sendMessage(message);
    }
}
void Channel::addMember(Client &client)
{
    if (!isMember(client))
    {
        members.push_back(client);
        broadcast(client.getNickName() + " has joined the channel.");
    }
}


bool Channel::isMember(Client &client)
{
    return std::find(members.begin(), members.end(), client) != members.end();
}
std::string Channel::getKey()
{
    return key;
}

void Channel::invite(Client &client)
{
    if (!isInvited(client))
    {
        invited.push_back(client);
        client.sendMessage("You have been invited to " + name_channel);
    }
}


std::vector<Client> Channel::getMembers(){return members; }