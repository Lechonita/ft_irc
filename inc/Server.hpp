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
	private:
		std::string							_buffer;
		const std::string					_port;
		const std::string					_password;
		int									_nbClients;
		int									_serverSocket; // file descriptor that can be used for communication
		int									_bind; // assign an IP address and port to the socket
		int									_listen; // marks a socket as passive - used to accept connections
		std::vector<pollfd>					_pollFd; // poll descriptors, one per client connection
		std::map<int, Client>				_clientMap; // map of client sockets
		std::map<std::string, Channel>		_channelMap; // map of channel sockets


	public:
		Server(const std::string &port, const std::string &password);
		~Server();
		void	printAll(); //provisoire, a supprimer
		// Getters
		int									getSocketFd() const;
		std::string							getPassword() const;
		std::map<int, Client>				getClientMap() const;
		std::map<std::string, Channel>		getChannelMap() const;


		// Run server functions
		void								runServer();
		void								createNewClient();
		void								getClientMessage();
		void								interpretMessage(const std::map<int, Client>::iterator &client);

		// Send functions
		void								sendUserInformation(const std::vector<std::string> parameters, const Client& client);
		void								sendChannelInformation(const std::vector<std::string> parameters, const Client& client);
		void								sendMessageToReceivers(std::vector<std::string> receivers, std::string message, const Client& client);
		void								sendMessageToChannel(std::string receiver, std::string message, const Client& client);
		void								sendMessageToChannelNotSelf(std::string receiver, std::string message, const Client& client);
		void								sendMessageToUser(std::string receiver, std::string message, const Client& client);

		// Creation, invitation, addition
		void								inviteUser(const std::vector<std::string> parameters, Client& client, Server& server);
		void								addClientToChannel(std::string channel, std::string passwrd, Client& client, Server& server);
		void								createNewChannel(std::string channel_name, int client_socket, Server& server);
		void								createOrJoinChannel(std::vector<std::string> channels, std::vector<std::string> passwrds, Client& client, Server& server);

		// Remove functions
		void								removeClientsFromChannels(Client& client, std::vector<std::string> channels, std::vector<std::string> clients, std::string message);
		void								removeClientFromAllItsChan(Client& client);

		// Channel functions
		bool								isPartOfChannel(std::string channel_name, const Client& client);
		void								changeChannelsModes(Client& client, std::vector<std::string> channels, std::vector<std::string> modes_args, std::vector<std::string> modes_with_args, std::vector<std::string> modes_without_args);
		void								deleteChannel(std::string channel_name);

		// Client message reception
		void								manageClientMessageReception(const char *buffer, const int& clientSocket);
		void								setBuffer(const char *buffer);
		void								eraseBuffer();
		// void								interpretClientBuffer(const std::map<int, Client>::iterator &client, Server& server);

		// Client_disconnect
		void								disconnectClient(const int& clientSocket);
		void								removeClientFromPollFd(const int& clientSocket);
		void								removeClientFromMap(const int& clientSocket);

		// Signal
		static void							signalExit(int signum);



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
		class BadPasswordException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
		class ClientQuitException : public Server::Exception
		{
			public:
				const char *what() const throw();
		};
};

#endif