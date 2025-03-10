
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include<sstream>

#include "../include/Server.hpp"
#include "../include/Client.hpp"
void Client::setClientIp(int fd)
{
	struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char clientIp[INET_ADDRSTRLEN];

    if (getpeername(fd, (struct sockaddr *)&client_addr, &client_len) == -1)
        throw std::runtime_error("getpeername failed");
    if (inet_ntop(AF_INET, &client_addr.sin_addr, clientIp, sizeof(clientIp)) == NULL)
        throw std::runtime_error("inet_ntop failed");
    _clientIp = std::string(clientIp);
	std::cout << "Client IP: " << _clientIp << std::endl;
}
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

void Client::sendMessage(const std::string &message)
{
	if (send(fd, message.c_str(), message.size(), 0) < 0)
	{
		std::cerr << "Error sending message to client " << fd << std::endl;
	}
}
std::string Client::getPrefix()
{
    return (":" + nickname + "!" + username + "@" + _clientIp);
}

// Client::Client(int fd){
//     struct sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);
//     char clientIp[INET_ADDRSTRLEN];

//     if (getpeername(fd, (struct sockaddr *)&client_addr, &client_len) == -1)
//         throw std::runtime_error("getpeername failed");
//     if (inet_ntop(AF_INET, &client_addr.sin_addr, clientIp, sizeof(clientIp)) == NULL)
//         throw std::runtime_error("inet_ntop failed");
//     _clientIp = std::string(clientIp);
// }

std::set<std::string> Client::getJoinedChannels()
{
	return joinedChannels;
}

void Client::appendToBuffer(const std::string& data)
{ 
	messageBuffer += data;
}

std::string Client::getBuffer() const { 
	return messageBuffer; 
}

void Client::clearBuffer()
{ 
	messageBuffer.clear();
}
