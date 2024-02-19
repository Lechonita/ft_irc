#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

static bool isPasswordValid(const std::string &password)
{
	
}

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	  _password(password),
	  _socketfd(socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)),
	  _bind(bind(_socketfd, (struct sockaddr *)&_address, sizeof(_address))),
	  _listen(listen(_socketfd, MAX_CLIENTS)),
	  _acceptfd(accept(_socketfd, (struct sockaddr *)&_address, sizeof(_address)))
{
	if (isPasswordValid(password) == false)
		throw(InvalidPort());

	// SOCKET : This code creates a TCP socket for IPv6 communication
	// AF_INET6 = IPv6 Internet protocols
	// SOCK_STREAM = Two-way reliable communication (TCP)
	// IPPROTO_TCP = protocol for the protocol parameter , sinon 0
	if (_socketfd == ERROR)
		throw(SocketException());
	if (_bind == ERROR)
		throw(BindException());
	if (_listen == ERROR)
		throw(ListenException());
	if (_acceptfd == ERROR)
		throw(ConnectionException());
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

// void Server::printErrorMessage(const char *str) const
// {
// 	perror(str);
// }

/*************************************/
/*************** PUBLIC **************/
/*************************************/

// Exceptions

const char *Server::InvalidPort::what() const throw()
{
	return ("\033[0;31mError: Invalid port.\n\033[0m");
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
