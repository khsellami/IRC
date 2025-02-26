
#include "../include/header.hpp"
/*
	QUESTIONS:
		1.Is Receivers are separated by a comma (,), or by a comma and space?
		==>>Receivers are separated only by a comma (,)
		2.Is Receivers can be both users and channels in the same command?
		==>>Yes, the recipient list can contain both users and channels.
		3.Is this information correct : ``Message must start with : if it contains spaces``?
		==>>Yes, this is correct.
		4.i need to handle this case:``:message privmsg target``?
		5.what is the MAX_TARGETS allowed?
		6.if one of receiver is incorrect , i need to terminate send to what after him or send a reply and stop?
		==>>
		```
			If one of the receivers is invalid, you should:
				1.Send an error for the invalid recipient.
				2.Continue sending to the valid ones.
		```
		7.The command like that : ```privmsg user1,user2, :message ``` must give me what?
*/

/*
A recipient can be:

	1.A single user
	2.Multiple users/channels (comma-separated)
	3.A channel (#channel)
	4.A host mask (#mask) (Operators only)
	5.A server mask ($mask) (Operators only)
*/


//****Extract Receivers***********************************//
std::vector<std::string> extract_recv(Msj msj)
{
	std::vector<std::string> receivers;
	std::string targetList = msj.args[1];
	std::stringstream ss(targetList);
	std::string target;
	while(std::getline(ss, target, ','))
	{
		if (!target.empty()) // To serach i need to do what if there is an empty target in the command
			receivers.push_back(target);
	}
	return receivers;
}
/////////////////////SEND////////////////////////////////////////////////////////////////////////////////////
//****send Channel***********************************//

//****Send User***********************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//privmsg target Message
//target privmsg Message
void handle_privmsg(Server &server, Client &client, Msj msj)
{
	(void)server;
	(void)client;
	//****just for debug***********************************//
	std::vector<std::string>::iterator it = msj.args.begin();
	while (it != msj.args.end())
	{
		std::cout << "[" << *it << "] ";
		it++;
	}
	std::cout << '\n';
	std::cout << "*********Inside privmsg Command*******************\n";
	//****Check if the presence of target***********************************//
	if (msj.args.size() < 2)
	{
		std::cout << "No Target Provided ERR_NORECIPIENT\n";
		return ;
	}
	//****Check if the presence of Message***********************************//
	if (msj.args.size() < 3)
	{
		std::cout << "No Message Provided ERR_NOTEXTTOSEND\n";
		return ;
	}

	//****Extract Receivers***********************************//
	std::vector<std::string> receivers;
	receivers = extract_recv(msj);
	//Add case : number receivers depasse maximum target allowed ==>> ERR_TOOMANYTARGETS
	//****Extract Message***********************************//
	// std::string message= extract_msg(msj);//handle this case in message:Message must start with : if it contains spaces
	//****just for debug : receivers***********************************//
	std::cout << "=======>>>>>>>Receivers\n";
	for(size_t i=0; i < receivers.size(); i++)
	{
		std::cout << "[" << receivers[i] << "] ";
		if (receivers[i][0] == '#' || receivers[i][0] == '&')
		{
			// send_channel();//params:receivers[i],server,message
			/*
			send_channel() pseudo-code:
				1.extract name channel
				2.check in channel exit in channels in server ==>> ERR_NOSUCHNICK 
				3.check if the client is member of the channel
				4.check if the client has permessions to send message==>> ERR_CANNOTSENDTOCHAN
				5.broadcast message 
			*/
		}
		else
		{
			// send_user();
			/*
			send_user() pseudo-code:
				1.check if the nick name of user exist in the server ==>> ERR_NOSUCHNICK
				2.check if the client has permessions to send message==>> ERR_CANNOTSENDTOCHAN
				3.send message to the user
			*/
		}
	}
	std::cout << '\n';
}

/*
#mask (operators only):
	1.If the mask is invalid ==>> ERR_WILDTOPLEVEL
	2.If the mask does not contain a dot (.) ==>> ERR_NOTOPLEVEL
	3.Message is sent to all users whose hostname matches the mask
$mask(operators only)

*/