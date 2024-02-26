#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <sys/socket.h> // socket
# include <netinet/in.h>
# include <unistd.h>

class Server
{

	public:
		Server(const std::string &port, const std::string &password);
		~Server();

		// void printErrorMessage(const char *str) const;

	private:
		const std::string	_port;
		const std::string	_password;
		struct sockaddr_in _address;
		int _socketfd;	// file descriptor that can be used for communication
		int _bind;		// assign an IP address and port to the socket
		int _listen;		// marks a socket as passive - used to accept connections
		// int _acceptfd;	// extracts an element from a queue of connections (The queue created by listen) for a socket
		// accept(_socketfd, (struct sockaddr *)&_address, sizeof(_address))

		// Exceptions
		class Exception : public std::exception {
			public:
				const char *what() const throw();
		};
		class InvalidPort : public Server::Exception {
			public:
				const char *what() const throw();
		};
		class InvalidPassword : public Server::Exception {
			public:
				const char *what() const throw();
		};
		class SocketException : public Server::Exception {
			public:
				const char *what() const throw();
		};
		class BindException : public Server::Exception {
			public:
				const char *what() const throw();
		};
		class ListenException : public Server::Exception {
			public:
				const char *what() const throw();
		};
		class ConnectionException : public Server::Exception {
			public:
				const char *what() const throw();
		};
};

#endif