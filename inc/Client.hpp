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

	private:
		std::string					_clientUsername;
		std::string					_clientPassword;
		std::string					_clientNickname;
		std::string					_clientOldNickname;
		std::string					_clientRealName;
		std::string					_lastArg;
		std::string					_lastCommand;
		// std::string					_buffer;
		std::vector<Channel*>		_channels;
		const char*					_clientIP;
		int							_clientSocket;
		int							_clientStatus;
		bool						_irssi;
		bool						_nicknameOK;


	public:
		Client(const int& serverSocket);
		~Client();

		void	printChannels();
		// Getters
		int							getClientSocket() const;
		int							getClientStatus() const;
		const std::string			getClientUsername() const;
		const std::string			getClientNickname() const;
		const std::string			getClientOldNickname() const;
		const std::string			getClientRealName() const;
		const std::string			getClientPassword() const;
		const std::string			getLastArgument() const;
		const std::string			getLastCommand() const;
		const char*					getClientIP() const;
		const std::vector<Channel*>	getClientChannels() const;
		bool						getIrssi() const;
		bool						getNicknameOKFlag() const;


		// Setters
		// void						setBuffer(const char *buffer);
		void						setUsername(const std::string& username);
		void						setNickname(const std::string& nickname);
		void						setOldNickname(const std::string& oldnickname);
		void						setRealName(const std::vector<std::string>	parameters);
		void						setPassword(const std::string& password);
		void						setLastArgument(const std::string& arg);
		void						setLastCommand(const std::string& command);
		void						setClientIP(const char * IP);
		void						setClientStatus(const int login);
		void						setIrssi(const bool result);
		void						setNicknameOKFlag(const bool flag);

		// Functions
		// void						interpretMessage(Server& server);
		void						newChannel(Channel& channel_name);
		void						partFromChannels(Client& client, Server& server, const std::vector<std::string> channels, const std::string message = "");
		bool						isOperator(const std::string channelname) const;
		bool						userIsInChannel(const std::string& channelname, const std::string& usernickname) const;
		void						removeChannelFromClient(const Channel& channel);


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

};

#endif

/* authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.*/