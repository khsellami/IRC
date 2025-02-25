/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:50:06 by hmraizik          #+#    #+#             */
/*   Updated: 2025/02/25 23:38:05 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include <sstream>

void  check_Password(Client& client, std::string password, Msj command){
    if (command.args[0] == "PASS" && command.args[1] == password){
        client.is_PASS = true;
        std::cout << "PASSWORD verified!!\n";
    }
    else if (command.args[1] != password)
    {
        client.is_PASS = false;
        std::cout << "PASSWORD Incorrect!!\n";
    }
}

bool check_Dupplicated(Msj command, std::map<int, Client> clients){
    std::map<int, Client>::iterator i = clients.begin();

    while (i != clients.end())
    {
        if (i->second.getNickName() == command.args[1])
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
        if (!NickNameValide(command))
            std::cout << "throw: ERR_ERRONEUSNICKNAME\n";
        if (check_Dupplicated(command, clients))
            std::cout << "//throw: ERR_NICKNAMEINUSE\n";
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
            std::cout << "// throw: ERR_ALREADYREGISTERED\n";
            return ;
        }
        if (command.args[1].empty())
        {
            std::cout << "throw: ERR_NEEDMOREPARAMS\n";
            return ;
        }
        client.setUserName(command.args[1]);
        client.is_USER = true;
    }
}

int Wrong_args(Msj msj){
    if (msj.args.size() < 2)
    {
        return TOO_FEW_ARGS;
    }
    if (msj.args.size() > 2)
    {
        return TOO_MUCH_ARGS;
    }
    return 0;
}

void UpdateNickname(Client &client, Server &server, Msj msj, std::map<int , Client>& clients){
    (void)server;
    if (!NickNameValide(msj))
            std::cout << "//throw: ERR_ERRONEUSNICKNAME\n";
    if (check_Dupplicated(msj, clients))
        std::cout << "//throw: ERR_NICKNAMEINUSE\n";
    std::string OldNickname = client.getNickName();
    client.setNickName(msj.args[0]);
    std::cout << " // Broadcast to channels: :<oldnickname> NICK :<newnickname>\n";
    //     // This requires access to the channels the client is in
}

void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client>& clients, Server& server)
{
    // Already registred
    if (client.getIs_auth() == true)
    {
        if (msj.args[0] == "PASS" || msj.args[0] == "USER")
        {
            std::cout << "throw:`You are already connected and cannot handshake again`\n";
            return ;
        }
        else if (msj.args[0] != "NICK")
            return ;
    }
    if (msj.args[0] == "NICK" && client.is_NICK && client.is_PASS)
    {
        UpdateNickname(client, server, msj, clients);
    }
    
    /****** trying a command before registering *************************/
    if (client.getIs_auth() == false && msj.args[0] != "PASS" && msj.args[0] != "NICK" && msj.args[0] != "USER")
    {
            std::cout << "You can't execute command before registering!!!\n";
            return ;
    }
    if (!client.is_PASS && msj.args[0] != "PASS")
    {
        std::cout << "You must enter `PASS <password>` first\n";
        return ;
    }
    /******  checking arguments validation first ********************************/
    if (Wrong_args(msj))
    {
        if (Wrong_args(msj) == TOO_MUCH_ARGS)
        {
            std::cout << "// throw: not valid arguments: Too much\n";
        }
        else if (Wrong_args(msj) == TOO_FEW_ARGS)
        {
            std::cout << "// throw: not valid arguments: Too FEW\n";
            return ;
        }
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
        std::cout << "Welcome, You have registred succesfully!\n";
    }
}
