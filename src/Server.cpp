#include "../inc/Server.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	  _password(password),
	  _nbClients(0),
	  _pollFd(0),
	  _irssi(false)
{
	// SOCKET : This code creates a TCP socket for IPv6 communication
	// AF_INET6 = IPv6 Internet protocols
	// SOCK_STREAM = Two-way reliable communication (TCP)
	// IPPROTO_TCP = protocol for the protocol parameter , sinon 0

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == ERROR)
		throw(SocketException());

	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == ERROR)
	{
		close(_serverSocket);
		throw(BlockException());
	}

	std::istringstream iss(port);
	int portInt;
	iss >> portInt;

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portInt);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	_bind = bind(_serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (_bind == ERROR)
	{
		close(_serverSocket);
		throw(BindException());
	}

	_listen = listen(_serverSocket, MAX_CLIENTS);
	if (_listen == ERROR)
	{
		close(_serverSocket);
		throw(ListenException());
	}

	_pollFd.push_back(pollfd());
	_pollFd.back().fd = _serverSocket;
	_pollFd.back().events = POLLIN; // We want to monitor data reception on this file descriptor

	std::cout << GREEN << "\n       Server Creation Successful [Connected]" << NC << std::endl;
	std::cout << std::endl;
	// std::cout << "Server fd = " << _serverSocket
	// 		<< " / ip = " << inet_ntoa(serverAddress.sin_addr)
	// 		<< " / port = " << ntohs(serverAddress.sin_port) << std::endl;
}



Server::~Server()
{
	// _clientMap.clear();
	// std::cout << "Default server destructor." << std::endl;
}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

/*************************************/
/*************** PUBLIC **************/
/*************************************/

// Debug

// static void	printClientMap(const std::map<int, Client>  &clientMap)
// {
// 	std::map<int, Client>::const_iterator it;

// 	for (it = clientMap.begin(); it != clientMap.end(); ++it)
// 	{
// 		std::cout << "ClientMap #" << it->first << " : " << it->second.getClientSocket() << std::endl;
// 	}
// }


// Functions


void	Server::runServer()
{
	if (poll(&_pollFd[0], _pollFd.size(), TIMEOUT) == ERROR)
	{
		if (doSignal == false)
		{
			close(_serverSocket);
			throw(PollException());
		}
	}

	if (_pollFd[0].revents == POLLIN)
	{
		try
		{
			createNewClient();
		}
		catch (Server::Exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	else
	{
		getClientMessage();
	}
}


void	Server::createNewClient()
{
	struct	sockaddr_in clientAddress;
	socklen_t			size = sizeof(clientAddress);

	int	clientSocket = accept(_pollFd[0].fd, (struct sockaddr *)&clientAddress, &size);
	if (clientSocket == ERROR)
	{
		close(_serverSocket);
		throw(AcceptException());
	}

	// Make the client socket non-blocking
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == ERROR)
	{
		close(_serverSocket);
		close(clientSocket);
		throw(BlockException());
	}

	std::cout << "[ New connection ] Client fd = " << clientSocket
			<< "  /  ip = " << inet_ntoa(clientAddress.sin_addr)
			<< "  /  port = " << ntohs(clientAddress.sin_port) << std::endl;

	// Create the new client object and store it in std::map
	Client	newClient(clientSocket);
	newClient.setClientIP(inet_ntoa(clientAddress.sin_addr));
	_clientMap.insert(std::make_pair(clientSocket, newClient));
	_nbClients += 1;

	_pollFd.push_back(pollfd());
	_pollFd.back().fd = clientSocket;
	_pollFd.back().events = POLLIN; // We want to monitor data reception on this file descriptor

	// printClientMap(_clientMap);
}


void	Server::getClientMessage()
{
	if (_nbClients == 0 || _clientMap.size() == 0)
		return ;

	char	buffer[BUFFERSIZE];

	std::vector<pollfd>::iterator	it;
	for(it = _pollFd.begin(); it != _pollFd.end(); ++it)
	{
		if (it->revents == POLLIN)
		{
			int	clientSocket = it->fd;
			int	bytesRead = recv(clientSocket, buffer, BUFFERSIZE, 0);

			if (bytesRead == ERROR)
			{
				memset(buffer, 0, BUFFERSIZE);
				disconnectClient(clientSocket);
				return ;
			}
			else
			{
				buffer[bytesRead] = '\0';
				manageClientMessageReception(buffer, clientSocket);
				memset(buffer, 0, BUFFERSIZE);
			}
		}
	}
}



void	Server::removeClientfromServer(const Client& client)
{
	const size_t	clientSocket = client.getClientSocket();

	if (_clientMap.find(clientSocket) != _clientMap.end())
		_clientMap.erase(_clientMap.find(clientSocket));
}



// Getters

int									Server::getSocketFd() const { return (_serverSocket); }
std::string							Server::getPassword() const { return (_password); }
std::map<int, Client>				Server::getClientMap() const { return (_clientMap); }
std::map<std::string, Channel>		Server::getChannelMap() const { return (_channelMap); }
bool								Server::getIrssi()const { return (_irssi); }

// std::vector<std::string>	Server::getCommandList() const { return (_cmdMap); }


void	Server::printAll() //provisoire, a supprimer
{
	std::map<std::string, Channel>::iterator	it_channels;
	std::map<int, Client>::iterator				it_client;

	for (it_channels = _channelMap.begin() ; it_channels != _channelMap.end() ; it_channels++)
	{
		std::cout << GREEN << "channel= " << it_channels->second.getChannelName()
		<< ", channel address = " << &(it_channels->second) << NC << std::endl;
		it_channels->second.printClients();
	}

	for (it_client = _clientMap.begin() ; it_client != _clientMap.end() ; it_client++)
	{
		std::cout << RED << "client= " << it_client->first << ", " << it_client->second.getClientNickname()
		<< ", client address = " << &(it_client->second) << NC << std::endl;
		it_client->second.printChannels();
	}
}

// Setters

void	Server::setIrssi(const bool result) { _irssi = result; }

void	Server::createNewChannel(std::string channel_name, int client_socket, Server& server)
{
	std::map<int, Client>::iterator	it_client = _clientMap.find(client_socket);

	_channelMap.insert(std::make_pair(channel_name, Channel(channel_name, &(it_client->second))));
	it_client->second.newChannel(_channelMap.find(channel_name)->second);
	Utils::joinMessageSuccessful(it_client->second, server, channel_name);
}


void	Server::addClientToChannel(std::string channel, std::string passwrd, Client& client, Server& server)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(channel);

	it->second.newClient(passwrd, client);
	Utils::joinMessageSuccessful(client, server, channel);
}



void	Server::createOrJoinChannel(std::vector<std::string> channels, std::vector<std::string> passwrds, Client& client, Server& server)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != PREFIX_CHAN)
		{
			Utils::sendErrorMessage(ERR_NOSUCHCHANNEL, client, channels[i]);
		}
		else if (_channelMap.find(channels[i]) == _channelMap.end())
		{
			createNewChannel(channels[i], client.getClientSocket(), server);
		}
		else if (i >= passwrds.size())
		{
			addClientToChannel(channels[i], "", client, server);
		}
		else
		{
			addClientToChannel(channels[i], passwrds[i], client, server);
		}
	}
}



bool	Server::isPartOfChannel(std::string channel_name, const Client& client)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(channel_name);

	if (it == _channelMap.end())
	{
		Utils::sendErrorMessage(ERR_NOSUCHNICK, client);
		return (false);
	}
	std::vector<channelClient>	clients = it->second.getChannelClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].client->getClientNickname() == client.getClientNickname())
			return (true);
	}
	Utils::sendErrorMessage(ERR_NOTONCHANNEL, client, channel_name);
	return (false);
}



void	Server::sendMessageToReceivers(std::vector<std::string> receivers, std::string message, const Client& client)
{
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#' && isPartOfChannel(receivers[i], client) == true)
		{
			sendMessageToChannel(receivers[i], message, client);
		}
		else if (receivers[i][0] != '#')
		{
			sendMessageToUser(receivers[i], message, client);
		}
	}
}



void	Server::sendMessageToChannel(std::string receiver, std::string message, const Client& client)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(receiver);

	if (it == _channelMap.end())
	{
		Utils::sendErrorMessage(ERR_NOSUCHNICK, client);
		return ;
	}
	std::string	full_message = message + END_MSG;

	it->second.sendMessageToAll(full_message);
}



void	Server::sendMessageToUser(std::string receiver, std::string message, const Client& client)
{
	std::map<int, Client>::iterator	it;

	for(it = _clientMap.begin(); it != _clientMap.end(); it++)
	{
		if (it->second.getClientNickname() == receiver)
			break;
	}
	if (it == _clientMap.end())
	{
		Utils::sendErrorMessage(ERR_NOSUCHNICK, client);
		return ;
	}
	std::string	full_message = client.getClientNickname() + ": " + message+ END_MSG;

	Utils::sendMessage(full_message, it->second);
}



// Exceptions

const char *Server::Exception::what() const throw() { return (ERR_SERVER); }

const char *Server::SocketException::what() const throw() { return (ERR_SERVER_SOCKET); }

const char *Server::BindException::what() const throw() { return (ERR_SERVER_BIND); }

const char *Server::ListenException::what() const throw() { return (ERR_SERVER_LISTEN); }

const char *Server::ConnectionException::what() const throw() { return (ERR_SERVER_CONNECT); }

const char *Server::PollException::what() const throw() { return (ERR_SERVER_POLL); }

const char *Server::BlockException::what() const throw() { return (ERR_SERVER_BLOCK); }

const char *Server::AcceptException::what() const throw() { return (ERR_SERVER_ACCEPT); }

const char *Server::ParametersException::what() const throw() { return (ERR_NEEDMOREPARAMS); }


/*************************************/
/*************** PRIVATE *************/
/*************************************/
