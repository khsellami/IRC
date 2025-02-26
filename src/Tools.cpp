
#include "../include/header.hpp"

//****Extract the message form the command***********************************//
std::string geting_message(const std::string &msg)
{
	size_t pos = msg.find(":");
	return (pos != std::string::npos) ? msg.substr(pos + 1) : "";
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
		toSend[i].sendMessage(message);
	}
}
