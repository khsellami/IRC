#ifndef HEADER_HPP
#define HEADER_HPP
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Msj.hpp"
#include "Reply.hpp"

void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client>& clients, Server& server);
void parse_message(const std::string &msg1, Client &client, const char* password, std::map<int , Client> clients, Server &server, std::map<std::string, Channel> channels);
//TOPIC///////////////////////////////////////////////////
void handle_topic(Server &server, Client &client, Msj msj);
///////////////////////////////////////////////////////////
void handle_join(Server &server, Client &client, Msj &msj);
void handle_invite(Server &server, Client &client, Msj &msj);
void handle_authentification(Client &client, std::string password, Msj msj, std::map<int , Client> clients);
void broadcastMessage(Client &client, Channel &channel, std::string message);
std::string geting_message(const std::string &msg);


#endif
