#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <vector>
# include <poll.h>
# include <sstream>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iostream>
# include <exception>
# include <sys/poll.h>
# include <sys/socket.h>
# include <netinet/in.h>

# include "../inc/Client.hpp"
# include "../inc/Channel.hpp"

class Client;

class Channel;

class Server
{

	public:

		Server(const std::string &port, const std::string &password);
		~Server();

		// Getters
		int getSocketFd() const;
		std::string getPassword() const;

		// Functions
		void runServer();
		void createNewClient();
		void getClientMessage();

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
		class AcceptException : public Server::Exception
		{
		public:
			const char *what() const throw();
		};
		class BlockException : public Server::Exception
		{
		public:
			const char *what() const throw();
		};


	private:

		const std::string		_port;
		const std::string		_password;
		int						_nbClients;
		int						_serverSocket;						// file descriptor that can be used for communication
		int						_bind;								// assign an IP address and port to the socket
		int						_listen;							// marks a socket as passive - used to accept connections
		std::vector<pollfd>		_pollFd;			// poll descriptors, one per client connection
		std::map<int, Client>	_serverClients;	// map of client sockets
		std::map<int, Channel>	_serverChannels; // map of channel sockets
};

#endif