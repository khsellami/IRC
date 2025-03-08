#ifndef HEADER_HPP
#define HEADER_HPP
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Msj.hpp"
#include "Reply.hpp"

void handle_authentification(Client &client, Msj msj, Server& server);
void handleChannelMode(Client &client, Msj msj, Server &server);
void parse_message(const std::string &msg1, Client &client, Server &server);

///////////////////////////////////////////////////////////
//TOPIC
void handle_topic(Server &server, Client &client, Msj msj);
//PRIVMSG
void handle_privmsg(Server &server, Client &client, Msj msj);
///////////////////////////////////////////////////////////

void handle_join(Server &server, Client &client, Msj &msj);
void handle_invite(Server &server, Client &client, Msj &msj);
void broadcastMessage(Client &client, Channel &channel, std::string message);
std::string geting_message(const std::string &msg);
std::string toUpper(const std::string &str);
bool handle_dcc_message(std::string message, Client &sender, Client &receiver, Server &server);

#endif
