#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "Client.hpp"

class Channel
{
	private:
		std::string name_channel;
		std::string topic;
		std::vector<Client> members;
		///MODE
		// bool t;
		// bool i;
		// bool k;
		// bool l;
	public:

};

#endif
