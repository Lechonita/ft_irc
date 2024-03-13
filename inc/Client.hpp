#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/Utils.hpp"
#include "../inc/defines.hpp"
#include "../inc/Server.hpp"

class Server;

class Client
{

	public:
		Client(const int& serverSocket);
		~Client();


	// Getters
	int						getClientSocket() const;
	std::string				getClientUsername() const;
	std::string				getClientNickname() const;
	std::string				getClientPassword() const;
	const std::string		getLastArgument() const;
	const std::string		getLastCommand() const;
	const char*				getClientIP() const;


	// Setters
	void			setClientIP(const char * IP);
	void			setBuffer(const char *buffer);
	void			setNickname(const std::string& nickname, const bool irssi);
	void			setPassword(const std::string& password, const bool irssi);
	void			setUsername(const std::string& username, const bool irssi);
	void			setLastArgument(const std::string& arg);
	void			setLastCommand(const std::string& command);

	// Functions
	void			interpretMessage(Server& server);


	// Commands
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
		std::string		_lastArg;
		std::string		_lastCommand;
		std::string		_buffer;
		const char*		_clientIP;
		int				_clientSocket;
		// bool			_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/