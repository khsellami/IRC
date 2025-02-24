
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

int Client::getIs_auth()
{
	return is_auth;
}