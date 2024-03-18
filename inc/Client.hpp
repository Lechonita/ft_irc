#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/Utils.hpp"
#include "../inc/defines.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"

class Channel;

class Server;

class Client
{

	public:
		Client(const int& serverSocket);
		~Client();


	// Getters
	int							getClientSocket() const;
	const std::string			getClientUsername() const;
	const std::string			getClientNickname() const;
	const std::string			getClientPassword() const;
	const std::string			getLastArgument() const;
	const std::string			getLastCommand() const;
	const char*					getClientIP() const;
	const std::vector<Channel*>	getClientChannels() const;
	bool						getClientStatus() const;


	// Setters
	void						setBuffer(const char *buffer);
	void						setUsername(const std::string& username, const bool irssi);
	void						setNickname(const std::string& nickname, const bool irssi);
	void						setPassword(const std::string& password, const bool irssi);
	void						setLastArgument(const std::string& arg);
	void						setLastCommand(const std::string& command);
	void						setClientIP(const char * IP);
	void						setClientStatus(const bool& connected);

	// Functions
	void						interpretMessage(Server& server);
	void						newChannel(Channel& channel_name);

	// Commands
	// bool					isCommandFromList(const std::string& command, const Server& server) const;


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
		std::string					_clientUsername;
		std::string					_clientPassword;
		std::string					_clientNickname;
		std::string					_lastArg;
		std::string					_lastCommand;
		std::string					_buffer;
		std::vector<Channel*>		_channels;
		const char*					_clientIP;
		int							_clientSocket;
		bool						_clientStatus;
};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/