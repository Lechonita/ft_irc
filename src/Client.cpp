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
	std::cout << "WELCOME\n" << std::endl;
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

int							Client::getClientSocket() const { return (_clientSocket); }
const std::string			Client::getClientUsername() const { return (_clientUsername); }
const std::string			Client::getClientNickname() const { return (_clientNickname); }
const std::string			Client::getClientPassword() const { return (_clientPassword); }
const std::string			Client::getLastArgument() const { return (_lastArg); }
const std::string			Client::getLastCommand() const { return (_lastCommand); }
const char*					Client::getClientIP() const { return (_clientIP); }
const std::vector<Channel*>	Client::getClientChannels() const {return (_channels);}

// Setters

void	Client::printChannels()
{
	for (size_t i = 0; i < _channels.size(); i++)
		std::cout << GREEN << "	channel " << i << "= " << _channels[i]->getChannelName()
		<< ", channel address = " << (_channels[i]) << NC << std::endl;
}

void	Client::setClientIP(const char * IP) { _clientIP = IP; }

void	Client::setBuffer(const char *buffer) { _buffer += buffer; }



void	Client::setPassword(const std::string& password, const bool irssi)
{
	_clientPassword = password;
	if (irssi == false)
		Utils::sendMessage(PASS_OK, *this);
}



void	Client::setNickname(const std::string& nickname, const bool irssi)
{
	if (_clientNickname.empty() == false)
	{
		Utils::sendMessage(NICK_CHANGED, *this);
		_clientNickname.clear();
	}
	else
	{
		if (irssi == false)
			Utils::sendMessage(NICK_OK, *this);
	}
	_clientNickname = nickname;
}

void			Client::setLastArgument(const std::string& arg) {_lastArg = arg;}
void			Client::setLastCommand(const std::string& command) {_lastCommand = command;}


void	Client::setUsername(const std::string& username, const bool irssi)
{
	_clientUsername = username;
	if (irssi == false)
		Utils::sendMessage(USERNAME_OK, *this);
}

// Functions

void	Client::interpretMessage(Server& server)
{
	if (_buffer.empty() == true)
		return ;

	size_t	pos = _buffer.find("\r\n");
	if (pos == std::string::npos) // no \r\n found
		pos = _buffer.find("\n");

	while (pos != std::string::npos)
	{
		std::string	line = _buffer.substr(0, pos);
		std::cout << INCOMING_MSG << line << std::endl;

		if (line.empty() == false)
			Commands::findCommandInMessage(line, server, *this);

		_buffer.erase(0, _buffer.find("\n") + 1);
		pos = _buffer.find("\r\n");
		if (pos == std::string::npos)
			pos = _buffer.find("\n");
	}
}



void	Client::newChannel(Channel& channel)
{
	_channels.push_back(&channel);
}



void	Client::partFromChannels(Client& client, Server& server, const std::vector<std::string> channels, const std::string message)
{
	std::vector<Channel*>::iterator	it;

	for (size_t i = 0; i < channels.size(); i++)
	{
		for (it = _channels.begin(); it != _channels.end(); it++)
		{
			if (channels[i] == (*it)->getChannelName())
			{
				(*it)->removeClient(client);
				_channels.erase(it);
				Utils::partMessage(client, server, channels[i], message);
				break;
			}
		}
	}
}

// Exceptions

const char *Client::Exception::what() const throw() { return (ERR_CLIENT); }

const char *Client::BlockException::what() const throw() { return (ERR_CLIENT_BLOCK); }
