#include "../inc/Client.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Client::Client(const int& clientSocket): _clientSocket(clientSocket)
{
	std::cout << std::endl;
	std::cout << "WELCOME" << std::endl;
	std::cout << ORANGE << "You are not connected to the server yet." << NC << std::endl;
	std::cout << "Use command PASS followed by <password> to connect." << std::endl;
}


Client::~Client()
{
	// std::cout << "Client fd " << _clientSocket << " destroyed." << std::endl;
}


/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

// Getters

int		Client::getClientSocket() const { return (_clientSocket); }


// Exceptions

const char *Client::Exception::what() const throw()
{
	return ("\033[0;31mDefault Client Exception\n\033[0m");
}

const char *Client::BlockException::what() const throw()
{
	return ("\033[0;31mError: Could not set client I/O operations to non-blocking.\n\033[0m");
}
