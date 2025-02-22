#ifndef USER_HPP
#define USER_HPP
#include <iostream>
class User
{
	private:
		int			fd;
		std::string host;

	public:

		User()
		{
		}
		void setSocket(int fd)
		{
			this->fd = fd;
		}
		void setHost(std::string host)
		{
			this->host = host;
		}
};

#endif