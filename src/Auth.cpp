/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:50:06 by hmraizik          #+#    #+#             */
/*   Updated: 2025/02/27 18:03:09 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include <sstream>



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
        }
        if (!NickNameValide(command))
        {
            client.sendMessage(ERR_ERRONEUSNICKNAME(command.args[1]));
        }
        if (check_Dupplicated(command, clients))
        {
            client.sendMessage(ERR_NICKNAMEINUSE(command.args[1]));
        }
        else
        {
            client.setNickName(command.args[1]);
            client.is_NICK = true;
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

void UpdateNickname(Client &client, Server &server, Msj msj, std::map<int , Client>& clients){
    (void)server;
    if (!NickNameValide(msj))
    {
        client.messageToSend = "432 " + client.getNickName() + " :Erroneus nickname\n";
        client.sendMessage(client.messageToSend);
    }
    if (check_Dupplicated(msj, clients))
    {
        client.messageToSend = "433 " + client.getNickName() + " :Nickname is already in use\n";
        client.sendMessage(client.messageToSend);
    }
    std::string OldNickname = client.getNickName();
    client.setNickName(msj.args[0]);
    client.sendMessage(std::string(" // Broadcast to channels: :<oldnickname> NICK :<newnickname>\n"));
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

void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client>& clients, Server& server)
{
    // Already registred
    if (client.getIs_auth() == true)
    {
        if (msj.args[0] == "PASS" || msj.args[0] == "USER")
        {
            client.messageToSend = "462 :You may not reregister\n";
            client.sendMessage(client.messageToSend);
            return ;
        }
        else if (msj.args[0] != "NICK")
            return ;
    }
    if (msj.args[0] == "NICK" && client.is_NICK && client.is_PASS)
    {
        UpdateNickname(client, server, msj, clients);
    }
    /****** trying other command before registering *************************/
    if (client.getIs_auth() == false && msj.args[0] != "PASS" && msj.args[0] != "NICK" && msj.args[0] != "USER")
    {
            client.messageToSend = "451 :You have not registered\n";
            client.sendMessage(client.messageToSend);
            return ;
    }
    //
    
    if (!client.is_PASS && msj.args[0] != "PASS")
    {
        client.messageToSend = "You must enter `PASS <password>` first\n";
        client.sendMessage(client.messageToSend);
        return ;
    }
    if (!(client.getIs_auth()) && msj.args[0] == "PASS")
    {
        check_Password(client, password, msj);
        return ;
    }
    if (!(client.getIs_auth()) && client.is_PASS)
    {
        check_Names(client, msj, clients);
    }
    // verify if auth complited
    if (!(client.getIs_auth()) && client.is_PASS && client.is_NICK && client.is_USER)
    {
        client.setIs_auth(true);
        KillNicknameCollisions(client, clients);

        client.messageToSend = "Welcome, You have registred succesfully!\n";
        client.sendMessage(client.messageToSend);
    }
}