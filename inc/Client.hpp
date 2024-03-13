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
	const std::string		getClientUsername() const;
	const std::string		getClientNickname() const;
	const std::string		getClientPassword() const;
	const std::string		getLastArgument() const;
	const std::string		getLastCommand() const;


	// Setters
	void			setBuffer(const char *buffer);
	void			setNickname(const std::string& nickname);
	void			setPassword(const std::string& password);
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
		int				_clientSocket;
		// bool			_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/