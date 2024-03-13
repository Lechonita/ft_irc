#ifndef SERVER_HPP
#define SERVER_HPP

#include "../inc/Utils.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


class Client;

class Channel;

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server();

		// Getters
		int									getSocketFd() const;
		std::string							getPassword() const;
		std::map<int, Client>				getClientMap() const;
		std::map<std::string, Channel>		getChannelMap() const;
		// std::vector<std::string>	getCommandList() const;

		// Setters
		// std::vector<std::string>	setCommandList();

		//JOIN command
		void						setChannelMap(std::string channel_name, int client_socket);
		void						addClientToChannel(std::string channel, std::string passwrd, Client& client);
		void						manageChannel(std::vector<std::string> channels, std::vector<std::string> passwrds, Client& client);

		// Functions
		void						runServer();
		void						createNewClient();
		void						getClientMessage();

		// Client message reception
		void						manageClientMessageReception(const char *buffer, const int& clientSocket);
		void						setClientBuffer(const std::map<int, Client>::iterator &client, const char *buffer);
		void						interpretClientBuffer(const std::map<int, Client>::iterator &client, Server& server);

		// Client_disconnect
		void						disconnectClient(const int& clientSocket);
		void						removeClientFromPollFd(const int& clientSocket);
		void						removeClientFromMap(const int& clientSocket);



		// Debug
		// void	printClientMap(const std::map<int, Client>  &clientMap);

		// Exceptions
		class Exception : public std::exception
		{
			public:
				const char *what() const throw();
		};
		class SocketException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class BindException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class ListenException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class ConnectionException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class PollException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class BlockException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class AcceptException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class ParametersException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		// class ReadException : public Client::Exception
		// {
		// 	public:
		// 		const char *what() const throw();
		// };

	private:
		const std::string					_port;
		const std::string					_password;
		int									_nbClients;
		int									_serverSocket; // file descriptor that can be used for communication
		int									_bind; // assign an IP address and port to the socket
		int									_listen; // marks a socket as passive - used to accept connections
		std::vector<pollfd>					_pollFd; // poll descriptors, one per client connection
		// std::vector<std::string>			_cmdList; // command function pointers indexed
		std::map<int, Client>				_clientMap; // map of client sockets
		std::map<std::string, Channel>		_channelMap; // map of channel sockets
};

#endif