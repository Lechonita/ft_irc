#ifndef SERVER_HPP
#define SERVER_HPP

# include <sstream>
# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <iostream>
# include <exception>
# include <sys/socket.h>
# include <netinet/in.h>

class Server
{

	public:
		Server(const std::string &port, const std::string &password);
		~Server();


	// Getters
		int getSocketFd() const;
		std::string getPassword() const;


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

	private:

		const std::string	_port;
		const std::string	_password;
		int _socketfd;	// file descriptor that can be used for communication
		int _bind;		// assign an IP address and port to the socket
		int _listen;		// marks a socket as passive - used to accept connections


	// Functions
		bool	isPasswordValid(const std::string& password);
		bool	isPortValid(const std::string& port);
};

#endif