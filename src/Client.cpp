
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include<sstream>

#include "../include/Server.hpp"
#include "../include/Client.hpp"

Client::Client()
{
	is_auth = false;
	is_PASS = false;
	is_NICK = false;
	is_USER = false;
	has_received = false;
	Mode_in_channel = 0;
}

void Client::setSocket(int fd)
{
	this->fd = fd;
}

void Client::setHost(const std::string &host)
{
	this->host = host;
}

int Client::getSocket()
{
	return fd;
}

bool Client::getIs_auth()
{
	return is_auth;
}
void Client::setIs_auth(bool value)
{
	is_auth = value;
}
void Client::setNickName(std::string Nick)
{
	nickname = Nick;
}
void Client::setHostname(std::string Hostname)
{
	hostname = Hostname;
}
void Client::setRealname(std::string Realname)
{
	realname = Realname;
}
void Client::setServername(std::string Servername)
{
	servername = Servername;
}
std::string Client::getHostname()
{
	return hostname;
}
std::string Client::getRealname()
{
	return realname;
}
std::string Client::getServername()
{
	return servername;
}
std::string Client::getNickName()
{
	return nickname;
}
void Client::setUserName(std::string User)
{
	username = User;
}

std::string Client::getName(){
	return this->username;
}

bool Client::isOp(){return is_op;}
void Client::sendMessage(const std::string &message)
{
    std::string formattedMessage = message + "\r\n"; // Format IRC
    if (write(fd, formattedMessage.c_str(), formattedMessage.length()) == -1)
    {
        std::cerr << "Error sending message to client " << fd << std::endl;
    }
}
std::string Client::getPrefix()
{
    return ":" + nickname + "!user@host";
}

std::set<std::string> Client::getJoinedChannels()
{
	return joinedChannels;
}
