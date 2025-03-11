#include "../include/Channel.hpp"

Channel::Channel()
{
    is_hasKey = false;
    topicRestricted = false;
    inviteOnly = false;
    is_limit = false;
    limit = 0;
}

Channel::Channel(std::string name) : name_channel(name) ,inviteOnly(false)
{
    is_hasKey = false;
    topicRestricted = false;
    inviteOnly = false;
    is_limit = false;
    limit = 0;
}

void Channel::setLimit(size_t newLimit)
{
    if (newLimit > 0)
    {
        limit = newLimit;
        is_limit = true;
    }
}

void Channel::removeLimit()
{
    is_limit = false;
    limit = 0;
}

std::string  Channel::getName()
{
    return name_channel;
}

bool Channel::hasKey() { return !key.empty(); }

bool Channel::iSInviteOnly()
{
    return inviteOnly; 
}

void Channel::setTopicRestriction(bool status)
{
    topicRestricted = status;
}

bool Channel::getTopicRestriction()
{
    return topicRestricted;
}

void Channel::setKey(std::string newKey)
{
    key = newKey; 
    is_hasKey = true;
}

void Channel::removeKey()
{
   is_hasKey = false;
   key.clear();
}

std::string Channel::getModeString()
{
    std::string modeString = "+";
    
    if (inviteOnly)
        modeString += "i";
    if (topicRestricted)
        modeString += "t";
    if (is_hasKey)
        modeString += "k";
    
    return modeString;
}

void Channel::__setOperator(std::string Nickname, bool AddorRemove)
{
    std::vector<Client>::iterator it;
    for (it = members.begin(); it != members.end(); it++){
        if (it->getNickName() == Nickname)
        {
            break;
        }
    }
    if (it == members.end() || !isMember(*it))
        return ;
    if (AddorRemove && std::find(operators.begin(), operators.end(), *it) == operators.end())
    {
        operators.push_back(*it);
        return ;
    }
    else if (!AddorRemove && (it = std::find(operators.begin(), operators.end(), *it)) != operators.end())
        operators.erase(it);
}

void Channel::setInviteOnly(bool status)
{
    inviteOnly = status; 
}

void Channel::setName(std::string name)
{
    name_channel = name;
}

bool Channel::isBanned(Client &client)
{
    return std::find(bannedUsers.begin(), bannedUsers.end(), client.getName()) != bannedUsers.end();
}

void Channel::removeMember(Client &client)
{
    std::vector<Client>::iterator it;
    for (it = members.begin(); it != members.end(); it++)
    {
        if (it->getNickName() == client.getNickName())
        {
            members.erase(it);
            break;
        }
    }
}

std::string Channel::getUserList()
{
    std::string list;
    for (size_t i = 0; i < members.size(); i++)
    {
        list += members[i].getName() + " ";
    }
    return list;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

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
    }
}

bool Channel::isMember(Client &client)
{
    for (size_t i = 0; i < members.size(); i++)
    {
        if (members[i].getSocket() == client.getSocket())
            return true;
    }
    return false;
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

std::vector<Client> Channel::getMembers()
{
    return members;
}

void Channel::setOperator(Client &client)
{
    if (!isMember(client))
        return;
    if (std::find(operators.begin(), operators.end(), client) == operators.end())
        operators.push_back(client);
}
