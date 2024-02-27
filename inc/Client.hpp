#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{

	public:
		Client();
		~Client();

	private:
		std::string		_clientUsername;
		std::string		_clientPassword;
		std::string		_clientNickname;
		int				_clientSocket;
		bool			_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/