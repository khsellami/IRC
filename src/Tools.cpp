
#include "../include/header.hpp"

//****Extract the message form the command***********************************//
std::string geting_message(const std::string &msg)
{
	size_t pos = msg.find(":");
	return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
}

//****Function to send message to a specific fd***********************************//
//I add this to check the value of retour of send
void sendMessage(int socket, std::string message)
{
	ssize_t bytesSent = send(socket, message.c_str(), message.length(), 0);
	if (bytesSent == -1)
	{
		std::cout << "Throe exception :ERROR when send \n";
	}
}

//****Client send message to All members channel expect him***********************************//
void broadcastMessage(Client &client, Channel &channel, std::string message)
{
	//Extract the members of the channel
	std::vector <Client> toSend = channel.getMembers();
	for (size_t i=0 ; i < toSend.size(); i++)
	{
		//skip the client that send the message to the channel
		if (toSend[i].getSocket() == client.getSocket())
			continue;	
		sendMessage(toSend[i].getSocket(), message);
	}
}
