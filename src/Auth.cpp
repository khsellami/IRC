
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include <sstream>

void  check_Password(Client& client, std::string password, Msj command){
    if (command.args[0] == "PASS" && command.args[0] == password){
        client.is_PASS = true;
        std::cout << "PASSWORD verified!!\n";
    }
    else if (command.args[0] != password)
    {
        client.is_PASS = false;
        std::cout << "PASSWORD Incorrect!!\n";
    }
}

bool check_Dupplicated(){
    return false;
}

void check_Names(Client& client, Msj command){
    if (command.args[0] == "USER")
    {
        if (client.is_USER)
        {
            // throw: `You are already connected and cannot handshake again`
            return ;
        }
        client.setUserName(command.args[1]);
        client.is_USER = true;
    }
    if (command.args[0] == "NICK")
    {
        if (!check_Dupplicated())
        {
            client.setNickName(command.args[1]);
            client.is_NICK = true;
        }
        // else
            //throw: Nick Name already in Use;
    }
}

int Wrong_args(Msj msj){
    if (msj.args.size() < 1)
    {
        return 1;
    }
    if (msj.args.size() > 1)
    {
        return 2;
    }
    return 0;
}
void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client> clients)
{
	
    
    std::cout << "Authenticating client: " << client.getSocket() << '\n';
    (void)clients;

    
    if (client.getIs_auth())
    {
        if (msj.args[0] == "PASS" || msj.args[0] == "USER")
        {
            //throw: `You are already connected and cannot handshake again`;
            return ;
        }
    }

    
    /****** trying a command before authentification *************************/

    if (msj.args[0] != "PASS" && msj.args[0] != "NICK" && msj.args[0] != "USER")
    {
        if (client.getIs_auth() == false)
        {
            // std::cout << "u can't execute command before authentification!!!\n";
            //throw: "u can't execute command before authentification!!!  "
            return;
        }
        else
            return;
    }

    /******  checking arguments validation first ********************************/
    if (Wrong_args(msj))
    {
        if (Wrong_args(msj) == 2)
        {
            //throw: not valid arguments: Too much
        }
        else if (Wrong_args(msj) == 1)
        {
            return;
            //throw: not valid arguments: Not enough
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
        check_Names(client, msj);
    }

    // verify if auth complited
    if (client.is_PASS && client.is_NICK && client.is_USER)
    {
        client.setIs_auth(true);
    }

    // the client can send PASS command more than one time and Just the last one is used for verification
    // if it already connected after verifying password and he try PASS : `You are already connected and cannot handshake again`
    // but the NICK if already setted and he tries again NICK : NICK name will updated.
	
}
