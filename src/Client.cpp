#include "../inc/Client.hpp"
#include "../inc/defines.hpp"
#include "../inc/Commands.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Client::Client(const int& clientSocket): _clientSocket(clientSocket)
{
	_clientUsername = EMPTY;
	_clientPassword = EMPTY;
	_clientNickname = EMPTY;
	_buffer = EMPTY;

	std::cout << std::endl;
	std::cout << "WELCOME" << std::endl;
	// std::cout << ORANGE << "You are not connected to the server yet." << NC << std::endl;
	// std::cout << "Use command PASS followed by <password> to connect." << std::endl;
}


Client::~Client()
{
	// std::cout << "Client fd " << _clientSocket << " destroyed." << std::endl;
}


/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

// Getters

int				Client::getClientSocket() const { return (_clientSocket); }
std::string		Client::getClientUsername() const { return (_clientUsername); }
std::string		Client::getClientNickname() const { return (_clientNickname); }
std::string		Client::getClientPassword() const { return (_clientPassword); }


// Setters

void	Client::setBuffer(const char *buffer) { _buffer += buffer; }
void	Client::setPassword(const std::string& password)
{
	_clientPassword = password;
	if (send(getClientSocket(), PASS_OK,  strlen(PASS_OK), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
}

void	Client::setNickname(const std::string& nickname)
{
	if (_clientNickname.empty() == false)
	{
		if (send(getClientSocket(), NICK_CHANGED,  strlen(NICK_CHANGED), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		_clientNickname.clear();
		_clientNickname = nickname;
	}
	else
	{
		if (send(getClientSocket(), NICK_OK,  strlen(NICK_OK), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		_clientNickname = nickname;
	}
	
}


// Functions

void	Client::interpretMessage(const Server& server)
{
	if (_buffer.empty() == true)
		return ;

	size_t	pos = _buffer.find("\r\n");
	if (pos == std::string::npos) // no \r\n found
		pos = _buffer.find("\n");

	while (pos != std::string::npos)
	{
		std::string	line = _buffer.substr(0, pos);

		if (line.empty() == false)
			Commands::findCommandInMessage(line, server, *this);

		_buffer.erase(0, _buffer.find("\n") + 1);
		pos = _buffer.find("\r\n");
		if (pos == std::string::npos)
			pos = _buffer.find("\n");
	}
}




// Exceptions

const char *Client::Exception::what() const throw() { return (ERR_CLIENT); }

const char *Client::BlockException::what() const throw() { return (ERR_CLIENT_BLOCK); }
