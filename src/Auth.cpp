/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:50:06 by hmraizik          #+#    #+#             */
/*   Updated: 2025/03/03 19:55:05 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/header.hpp"
#include <sstream>



void broadcastMessageForNick(Client &client, Channel &channel, std::string message)
{
	//Extract the members of the channel
	std::vector <Client> toSend = channel.getMembers();
	for (size_t i=0 ; i < channel.getMembers().size(); i++)
	{
		//skip the client that send the message to the channel
		if (channel.getMembers()[i].getSocket() == client.getSocket() || channel.getMembers()[i].has_received == true)
			continue;
        else if (channel.getMembers()[i].has_received == false)
        {
		    channel.getMembers()[i].sendMessage(message);
            channel.getMembers()[i].has_received = true;
        }
	}
}


void  check_Password(Client& client, std::string password, Msj command){
    if (command.args[0] == "PASS" && command.args[1] == password){
        client.is_PASS = true;
    }
    else if (command.args[1] != password)
    {
        client.is_PASS = false;
        client.sendMessage(ERR_INCORPASS(client.getNickName()));
    }
}

bool check_Dupplicated(Msj command, std::map<int, Client> clients){
    std::map<int, Client>::iterator i = clients.begin();

    while (i != clients.end())
    {
        if (i->second.getNickName() == command.args[1] && i->second.getIs_auth())
        {
            return true;
        }
        ++i;
    }
    return false;
}

bool NickNameValide(Msj command)
{
    if (command.args[1].empty())
        return false;

    if (!isalpha(command.args[1][0]))
        return false;
    
    for (size_t i = 1; i < command.args[1].length(); i++)
    {
        char c = command.args[1][i];
        if (isalnum(c))
            continue;
        
        if (c == '-' || c == '[' || c == ']' || c == '\\' || 
            c == '`' || c == '^' || c == '{' || c == '}')
            continue;
        
        return false;
    }
    return true;
}

void check_Names(Client& client, Msj command, std::map<int , Client>& clients){
    if (command.args[0] == "NICK")
    {
          if (command.args.size() < 2 || command.args[1].empty())
        {
            client.sendMessage(ERR_NONICKNAMEGIVEN(command.args[1]));
            return ;
        }
        if (!NickNameValide(command))
        {
            client.sendMessage(ERR_ERRONEUSNICKNAME(command.args[1]));
            return ;
        }
        if (check_Dupplicated(command, clients))
        {
            client.sendMessage(ERR_NICKNAMEINUSE(command.args[1]));
            return ;
        }
        else
        {
            client.setNickName(command.args[1]);
            client.is_NICK = true;
            return ;
        }
    }
    if (command.args[0] == "USER")
    {
        if (client.is_USER)
        {
            client.sendMessage(ERR_ALREADYREGISTRED(client.getName()));
            return ;
        }
        if (command.args.size() < 5)
        {
            client.sendMessage(ERR_NEEDMOREPARAMS(command.args[0]));
            return ;
        }
        client.setUserName(command.args[1]);
        client.setHostname(command.args[2]);
        client.setServername(command.args[3]);
        std::string realname;
        //concatenate real name arguments
        for(size_t j = 4; j < command.args.size(); j++)
        {
            if (j ==  4 && command.args[j][0] == ':')
            {
                realname = command.args[j].substr(1, command.args.size());//skipping ':'
                continue;
            }
            if (realname != "")
                realname = realname + " ";
            realname += command.args[j];
        }
        client.setRealname(realname);
        client.is_USER = true;
    }
}

void reset_hasReceivedBool(Server& server, Client& client){
    // std::map<std::string, Channel> channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = server.getChannels().begin();

    while (it != server.getChannels().end()){
        if (it->second.isMember(client))
        {
            // std::vector <Client> clients = it->second.getMembers();
            for (size_t i = 0; i < it->second.getMembers().size(); i++)
            {
                it->second.getMembers()[i].has_received = false;
            }
        }
        ++it;
    }
}
void prodcastNickUpdated(Server &server, Client& client, std::string oldNick)
{
    std::map<std::string, Channel>::iterator it = server.getChannels().begin();
    std::string message = oldNick + " changed his nickname to " + client.getNickName();
    while (it != server.getChannels().end())
    {
        std::cout << it->second.isMember(client) << std::endl;
        if (it->second.isMember(client))
        {
            broadcastMessageForNick(client, it->second, message);
        }
        ++it;
    }
}

void UpdateNickname(Client &client, Server &server, Msj msj, std::map<int , Client>& clients){
    if (!NickNameValide(msj))
    {
        client.sendMessage(ERR_ERRONEUSNICKNAME(msj.args[1]));
        return ;
    }
    if (check_Dupplicated(msj, clients))
    {
        client.sendMessage(ERR_NICKNAMEINUSE(msj.args[1]));
        return ;
    }
    std::string OldNickname = client.getNickName();
    client.setNickName(msj.args[1]);
    prodcastNickUpdated(server, client, OldNickname);
}

void KillNicknameCollisions(Client& client, std::map<int , Client>& clients)
{
    std::map<int, Client>::iterator i;
    for (i = clients.begin(); i != clients.end(); ++i){
        if (i->second.getNickName() == client.getNickName() &&  i->second.getSocket() != client.getSocket())
        {
            std::string message = "436 " + client.getNickName() + ":Nickname collision KILL\n";
            i->second.sendMessage(message);
            close(i->second.getSocket());
        }
    }
}

int handle_authentification(Client &client, Msj msj, Server& server)
{
    // Already registred
    msj.args[0] = toUpper(msj.args[0]);

    if (client.getIs_auth() == true)
    {
        if (msj.args[0] == "PASS" || msj.args[0] == "USER")
        {
            client.sendMessage(ERR_ALREADYREGISTRED(client.getNickName()));
            return 1;
        }
        else if (msj.args[0] != "NICK")
            return 0;
    }
    if (msj.args[0] == "NICK" && client.is_NICK && client.is_PASS)
    {
        UpdateNickname(client, server, msj, server.getClients());
    }
    /****** trying other command before registering *************************/
    if (client.getIs_auth() == false && msj.args[0] != "PASS" && msj.args[0] != "NICK" && msj.args[0] != "USER")
    {
            client.sendMessage(ERR_NOTREGISTERED(client.getNickName()));
            return 1;
    }
    //
    
    if (!client.is_PASS && msj.args[0] != "PASS")
    {
        std::string message = "451 " + client.getNickName() + " :You have not registered - PASS required first\r\n";
        client.sendMessage(message);
        return 1;
    }
    if (!(client.getIs_auth()) && msj.args[0] == "PASS")
    {
        check_Password(client, server.getPassword(), msj);
        return 1;
    }
    if (!(client.getIs_auth()) && client.is_PASS)
    {
        check_Names(client, msj, server.getClients());
    }
    // verify if auth complited
    if (!(client.getIs_auth()) && client.is_PASS && client.is_NICK && client.is_USER)
    {
        client.setIs_auth(true);
        KillNicknameCollisions(client, server.getClients());

        client.sendMessage(RPL_WELCOME(client.getNickName(), "Welcome to the `SERVER 9DIIM`"));
    }
    return 1;
}