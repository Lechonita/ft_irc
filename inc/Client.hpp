#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/defines.hpp"
#include "../inc/Server.hpp"

class Server;

class Client
{

	public:
		Client(const int& serverSocket);
		~Client();


	// Getters
	int				getClientSocket() const;
	std::string		getClientUsername() const;


	// Setters
	void			setBuffer(const char *buffer);


	// Functions
	void			interpretMessage(const Server& server);


	// Commands
	void			findCommandInMessage(const std::string& line, const Server& server);
	std::string		getCommandFromLine(const std::string& line) const;
	// bool			isCommandFromList(const std::string& command, const Server& server) const;


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
		std::string		_buffer;
		int				_clientSocket;
		// bool			_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/