#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/header.hpp"
#include "../include/Reply.hpp"




void handleChannelMode(Client &client, Msj msj, Server &server)
{
    //MODE #channel +t/i/o...

    std::string channel_name = (msj.args[1][0] == '#' || msj.args[1][0] == '&') ? msj.args[1].substr(1) : msj.args[1];
    Channel* channel = server.getChannel(channel_name);
    if (!channel) 
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(msj.args[1]));
        return ;
    }
    
    // this behavion i'm not sure if it obligation : just listing modes of the channel if no mode arguments
    if (msj.args.size() < 3) {
        std::string modeString = channel->getModeString();
        client.sendMessage("324 " + client.getNickName() + " " + msj.args[1] + " " + modeString + "\n");
        return;
    }
    //first thing, is it operator ?
    if (!channel->isOperator(client)) {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(msj.args[1]));
        return;
    }
    
    // Now i can process the mode
    std::string modes = msj.args[2];
    bool adding = true;
    unsigned int argIndex = 3;
    
    for (size_t i = 0; i < modes.length(); i++)
    {
        if (modes[i] == '+' && i == 0) {
            adding = true;
            continue;
        }
        else if (modes[i] == '-') {
            adding = false;
            continue;
        }// need more handling more cases in parse here '++t, ---t ...'

        switch(modes[i]) {
            case 'i':
                channel->setInviteOnly(adding);
                break;
            case 't':
                channel->setTopicRestriction(adding);
                break;
            case 'k':
                if (adding && argIndex < msj.args.size())
                    channel->setKey(msj.args[argIndex++]);
                else if (!adding)
                    channel->removeKey();
                break;
            case 'o':
                if (argIndex < msj.args.size())
                {
                    std::string targetNick = msj.args[argIndex++];
                    channel->__setOperator(targetNick, adding);
                }
                break;
            case 'l':
                if (adding && argIndex < msj.args.size())
                    channel->setLimit(atoi(msj.args[argIndex++].c_str()));
                else if (!adding)
                    channel->removeLimit();
                break;
            default:
                client.sendMessage("472 " + client.getNickName() + " " + std::string(1, modes[i]) + " :is unknown mode char\n");
        }
    }
    
    // Notify channel of mode changes
    channel->broadcast(":" + client.getNickName() + " MODE " + msj.args[1] + " " + modes + "\n");
}
//starting mode command based on RFC 1459