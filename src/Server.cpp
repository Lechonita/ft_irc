#include "Server.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Server::Server()
	: _port(NULL),
	_password(NULL),
	_socketfd(ERROR),
	_bind(ERROR),
	_listen(ERROR),
	_acceptfd(ERROR) {}

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	_password(password),
	_socketfd(socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)),
	_bind(bind(_socketfd, ))
{
	// SOCKET : This code creates a TCP socket for IPv6 communication
	// AF_INET6 = IPv6 Internet protocols
	// SOCK_STREAM = Two-way reliable communication (TCP)
	// IPPROTO_TCP = protocol for the protocol parameter , sinon 0
	if (_socketfd == ERROR) {
		throw(SocketException());
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

// void Server::printErrorMessage(const char *str) const
// {
// 	perror(str);
// }




/*************************************/
/*************** PUBLIC **************/
/*************************************/


// Exceptions

const char *Server::SocketException::what() const throw()
{
	return ("\033[0;31mGrade cannot be lower than 150.\n\033[0m");
}


