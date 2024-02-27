#include "../inc/Server.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	  _password(password)
{

	if (isPasswordValid(password) == false)
		throw(InvalidPassword());
	if (isPortValid(port) == false)
		throw(InvalidPort());

	// SOCKET : This code creates a TCP socket for IPv6 communication
	// AF_INET6 = IPv6 Internet protocols
	// SOCK_STREAM = Two-way reliable communication (TCP)
	// IPPROTO_TCP = protocol for the protocol parameter , sinon 0

	_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketfd == ERROR)
		throw(SocketException());


	std::istringstream iss(port);
	int portInt;
	iss >> portInt;

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portInt);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	_bind = bind(_socketfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (_bind == ERROR)
	{
		close(_socketfd);
		throw(BindException());
	}

	_listen = listen(_socketfd, MAX_CLIENTS);
	if (_listen == ERROR)
	{
		close(_socketfd);
		throw(ListenException());
	}

}

Server::~Server()
{
	std::cout << "Default destructor." << std::endl;
}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

/*************************************/
/*************** PUBLIC **************/
/*************************************/

int Server::getSocketFd() const { return (_socketfd); }

std::string	Server::getPassword() const { return (_password); }


// Exceptions

const char*		Server::Exception::what() const throw()
{
	return ("\033[0;31mDefault Server Exception\n\033[0m");
}

const char *Server::InvalidPort::what() const throw()
{
	return ("\033[0;31mError: Invalid port.\n\033[0m");
}

const char *Server::InvalidPassword::what() const throw()
{
	return ("\033[0;31mError: Invalid password.\n\033[0m");
}

const char *Server::SocketException::what() const throw()
{
	return ("\033[0;31mError: Invalid socket at server creation.\n\033[0m");
}

const char *Server::BindException::what() const throw()
{
	return ("\033[0;31mError: failed to bind port.\n\033[0m");
}

const char *Server::ListenException::what() const throw()
{
	return ("\033[0;31mError: Failed to listen to socket.\n\033[0m");
}

const char *Server::ConnectionException::what() const throw()
{
	return ("\033[0;31mError: Failed to grab connection.\n\033[0m");
}

/*************************************/
/*************** PRIVATE *************/
/*************************************/

bool Server::isPortValid(const std::string &port)
{
	(void)port;
	return (true);
}

bool Server::isPasswordValid(const std::string &password)
{
	(void)password;
	return (true);
}
