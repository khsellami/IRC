#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/header.hpp"
#include "../include/Reply.hpp"


bool isNumber(const std::string &str)
{
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

void handleChannelMode(Client &client, Msj msj, Server &server)
{
    std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
    Channel* channel = server.getChannel(channel_name);
    if (!channel)
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(msj.args[1]));
        return ;
    }
    if (msj.args.size() < 3) {
        std::string modeString = channel->getModeString();
        client.sendMessage(RPL_CHANNELMODEIS(client.getNickName(), msj.args[1], modeString));
        return;
    }
    if (!channel->isOperator(client)) {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(msj.args[1]));
        return;
    }
    std::string modes = msj.args[2];
    bool adding = true;
    unsigned int argIndex = 3;
    for (size_t i = 0; i < modes.length(); i++)
    {
        if (modes[i] == '+' && i == 0) {
            adding = true;
            continue;
        }
        else if (modes[i] == '-' && i == 0) {
            adding = false;
            continue;
        }
        switch(modes[i]) {
            case 'i':
                channel->setInviteOnly(adding);
                break;
            case 't':
                channel->setTopicRestriction(adding);
                break;
            case 'k':
                if (adding && argIndex < msj.args.size() && !msj.args[argIndex].empty())
                    channel->setKey(msj.args[argIndex++]);
                else if (!adding)
                    channel->removeKey();
                break;
            case 'o':
                if (argIndex >= msj.args.size())
                {
                    client.sendMessage(ERR_NONICKNAMEGIVEN(msj.args[argIndex]));
                    return;
                }
                if (argIndex < msj.args.size())
                {
                    Client *targetClient = server.getClientByName(msj.args[argIndex]);
                    if (!targetClient || !channel->isMember(*targetClient))
                    {
                        client.sendMessage(ERR_NOSUCHNICK(msj.args[argIndex]));
                        return;
                    }
                    std::string targetNick = msj.args[argIndex++];
                    channel->__setOperator(targetNick, adding);
                }
                break;
            case 'l':
                if (adding && argIndex < msj.args.size() && isNumber(msj.args[argIndex]))
                    channel->setLimit(atoi(msj.args[argIndex++].c_str()));
                else if (!adding)
                    channel->removeLimit();
                break;
            default:
                client.sendMessage(ERR_UNKNOWNMODE(client.getNickName(), msj.args[1], std::string(1, modes[i])));
                return;
        }
    }
    channel->broadcast(RPL_CHANGEMODE(client.getNickName(), msj.args[1], modes));
}
