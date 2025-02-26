#include "../include/Channel.hpp"

Channel::Channel(){}

Channel::Channel(std::string name) : name_channel(name) {}

std::string  Channel::getName() { return name_channel; }

void Channel::setName(std::string name) { name_channel = name; }

bool Channel::getT(){return t;}

void Channel::setTopic(std::string topic){this->topic = topic;}

void Channel::addMember(Client &client) { members.push_back(client); }

bool Channel::isMember(Client &client)
{
    for (std::vector<Client>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->getName() == client.getName()) {
            return true;
        }
    }
    return false;
}

void Channel::invite(Client &client) { invited.push_back(client); }

std::vector<Client> Channel::getMembers(){return members; }