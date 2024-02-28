#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/defines.hpp"

class Client
{

	public:
		Client(const int& serverSocket);
		~Client();


	// Getters
	int		getClientSocket() const;


	// Exceptions
	class Exception : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class BlockException : public Client::Exception
	{
		public:
			const char *what() const throw();
	};


	private:
		std::string		_clientUsername;
		std::string		_clientPassword;
		std::string		_clientNickname;
		int				_clientSocket;
		// bool			_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/