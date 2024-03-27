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
	_clientRealName = EMPTY;
	_lastArg = EMPTY;
	_lastCommand = EMPTY;
	_buffer = EMPTY;
	_clientStatus = DISCONNECTED;
	_irssi = false;

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
const std::string			Client::getClientOldNickname() const { return (_clientOldNickname); }
const std::string			Client::getClientRealName() const { return (_clientRealName); }
const std::string			Client::getClientPassword() const { return (_clientPassword); }
const std::string			Client::getLastArgument() const { return (_lastArg); }
const std::string			Client::getLastCommand() const { return (_lastCommand); }
const char*					Client::getClientIP() const { return (_clientIP); }
const std::vector<Channel*>	Client::getClientChannels() const {return (_channels);}
bool						Client ::getClientStatus() const { return (_clientStatus); }
bool						Client::getIrssi()const { return (_irssi); }



// Setters

void	Client::printChannels()
{
	for (size_t i = 0; i < _channels.size(); i++)
		std::cout << GREEN << "	channel " << i << "= " << _channels[i]->getChannelName()
		<< ", channel address = " << (_channels[i]) << NC << std::endl;
}

void	Client::setBuffer(const char *buffer) { _buffer += buffer; }
void	Client::setLastArgument(const std::string& arg) { _lastArg = arg; }
void	Client::setLastCommand(const std::string& command) { _lastCommand = command; }
void	Client::setClientIP(const char * IP) { _clientIP = IP; }
void	Client::setClientStatus(const bool& connected) { _clientStatus = connected; }
void	Client::setIrssi(const bool result) { _irssi = result; }


void	Client::setPassword(const std::string& password)
{
	_clientPassword = password;
	if (_irssi == false)
		Utils::sendMessage(PASS_OK, *this);
}



void	Client::setNickname(const std::string& nickname)
{
	if (_clientNickname.empty() == false)
	{
		setOldNickname(_clientNickname);
		_clientNickname.clear();
		_clientNickname = nickname;
		Utils::sendFormattedMessage(RPL_NICKCHANGE, *this);
		Utils::sendMessage(NICK_CHANGED, *this);
	}
	else
	{
		_clientNickname = nickname;
		if (_irssi == false)
			Utils::sendMessage(NICK_OK, *this);
	}
}



void	Client::setOldNickname(const std::string& oldnickname)
{
	if (_clientOldNickname.empty() == false)
		_clientOldNickname.clear();
	_clientOldNickname = oldnickname;
}



void	Client::setRealName(const std::vector<std::string>	parameters)
{
	const std::string	name = parameters[3].substr(1) + " " + parameters[4];
	_clientRealName = name;
}


void	Client::setUsername(const std::string& username)
{
	_clientUsername = username;
	if (_irssi == false)
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
		//il faudrait que cette fonction ne soit pas dans le client et le probleme est regle, on est dans un client qui n'existe plus et _buffer n'existe plus
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
	bool							just_removed = false;

	for (size_t i = 0; i < channels.size(); i++)
	{
		for (it = _channels.begin(); it != _channels.end(); it++)
		{
			if (channels[i] == (*it)->getChannelName())
			{
				Utils::partMessage(client, server, channels[i], message);
				(*it)->removeClient(client);
				if ((*it)->getChannelClients().size() == 0)
				{
					_channels.erase(it);
					server.deleteChannel((channels[i]));
					just_removed = true;
					break ;
				}
				_channels.erase(it);
				just_removed = true;
				break;
			}
		}
		if (just_removed == false && it == _channels.end())
		{
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, channels[i]);
		}
		just_removed = false;
	}
}



bool		Client::isOperator(const std::string channelname) const
{
	std::vector<Channel*>::const_iterator	it;

	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::vector<channelClient>				channelClients = (*it)->getChannelClients();
		std::vector<channelClient>::iterator	itClient;

		for (itClient = channelClients.begin(); itClient != channelClients.end(); ++itClient)
		{
			if ((*it)->getChannelName() == channelname && itClient->client->getClientNickname() == _clientNickname && itClient->isOperator == true)
				return (true);
		}
	}
	return (false);
}



bool		Client::userIsInChannel(const std::string& channelname, const std::string& usernickname) const
{
	std::vector<Channel*>::const_iterator	it;

	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::vector<channelClient>				channelClients = (*it)->getChannelClients();
		std::vector<channelClient>::iterator	itClient;

		for (itClient = channelClients.begin(); itClient != channelClients.end(); ++itClient)
		{
			if ((*it)->getChannelName() == channelname && itClient->client->getClientNickname() == usernickname)
				return (true);
		}
	}
	return (false);
}



void	Client::removeChannelFromClient(const Channel& channel)
{
	std::vector<Channel*>::iterator	it;

	for(it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it) == &channel)
		{
			_channels.erase(it);
			return ;
		}
	}
}




// Exceptions

const char *Client::Exception::what() const throw() { return (ERR_CLIENT); }

const char *Client::BlockException::what() const throw() { return (ERR_CLIENT_BLOCK); }
