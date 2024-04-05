#include "../inc/Server.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	  _password(password),
	  _pollFd(0)
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

	std::cout << GREEN_ITALIC << "\n[ New connection ] Client fd = " << clientSocket
			<< "  /  ip = " << inet_ntoa(clientAddress.sin_addr)
			<< "  /  port = " << ntohs(clientAddress.sin_port) << std::endl << NC_ITALIC;
	std::cout << std::endl;

	// Create the new client object and store it in std::map
	Client	newClient(clientSocket);
	newClient.setClientIP(inet_ntoa(clientAddress.sin_addr));
	_clientMap.insert(std::make_pair(clientSocket, newClient));

	_pollFd.push_back(pollfd());
	_pollFd.back().fd = clientSocket;
	_pollFd.back().events = POLLIN; // We want to monitor data reception on this file descriptor
}


void	Server::getClientMessage()
{
	if (_clientMap.size() == 0)
		return ;

	char	buffer[BUFFERSIZE];

	std::vector<pollfd>::iterator	it;
	for(it = _pollFd.begin(); it != _pollFd.end(); ++it)
	{
		if (it->revents == POLLIN)
		{
			int	clientSocket = it->fd;
			int	bytesRead = recv(clientSocket, buffer, BUFFERSIZE, 0);

			if (bytesRead == ERROR || bytesRead == DISCONNECTED)
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
				if (_clientMap.find(clientSocket)->second.getClientStatus() == DISCONNECTED)
				{
					disconnectClient(clientSocket);
					return ;
				}
			}
		}
	}
}



// Getters

int									Server::getSocketFd() const { return (_serverSocket); }
std::string							Server::getPassword() const { return (_password); }
std::map<int, Client>				Server::getClientMap() const { return (_clientMap); }
std::map<std::string, Channel>		Server::getChannelMap() const { return (_channelMap); }




void	Server::printAll() //provisoire, a supprimer
{
	std::map<std::string, Channel>::iterator	it_channels;
	std::map<int, Client>::iterator				it_client;

	for (it_channels = _channelMap.begin() ; it_channels != _channelMap.end() ; it_channels++)
	{
		std::cout << GREEN << "channel= " << it_channels->second.getChannelName()
		<< ", channel address = " << &(it_channels->second) << "pwd = " << it_channels->second.getChannelPass() << NC << std::endl;
		it_channels->second.printClients();
	}

	std::cout << std::endl;
	for (it_client = _clientMap.begin() ; it_client != _clientMap.end() ; it_client++)
	{
		std::cout << RED << "client= " << it_client->first << ", " << it_client->second.getClientNickname()
		<< ", client address = " << &(it_client->second) << ", status= " << it_client->second.getClientStatus() << NC << std::endl;
		it_client->second.printChannels();
	}
}



// Setters



void	Server::createNewChannel(std::string channel_name, int client_socket, Server& server)
{
	std::map<int, Client>::iterator	it_client = _clientMap.find(client_socket);

	_channelMap.insert(std::make_pair(channel_name, Channel(channel_name, &(it_client->second))));
	it_client->second.newChannel(_channelMap.find(channel_name)->second);
	Utils::joinMessageSuccessful(it_client->second, server, channel_name);
}



// TO BE ORGANIZED



// TO BE ORGANIZED


void	Server::inviteUser(const std::vector<std::string> parameters, Client& client, Server& server)
{
	const std::string	message = ":" + client.getClientNickname() + " INVITE " + parameters[0] + " " + parameters[1] + '\n';

	Utils::sendMessage(message, client);

	std::map<int, Client>::iterator		it;
	std::map<std::string, Channel>::iterator	it_chan = _channelMap.find(parameters[1]);

	for (it = _clientMap.begin(); it != _clientMap.end(); ++it)
	{
		if (it->second.getClientNickname() == parameters[0])
		{
			if (it_chan->second.getLMode() == true && it_chan->second.getUserLimit() <= it_chan->second.getChannelClients().size())
			{
				Utils::sendFormattedMessage(ERR_CHANNELISFULL, client, parameters[1]);
				return ;
			}
			else
				it_chan->second.newClient(it->second);
			Utils::joinMessageSuccessful(it->second, server, parameters[1]);
		}
	}
}

void	Server::addClientToChannel(std::string channel, std::string passwrd, Client& client, Server& server)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(channel);

	if (it->second.getLMode() == true && it->second.getUserLimit() <= it->second.getChannelClients().size())
	{
		Utils::sendFormattedMessage(ERR_CHANNELISFULL, client, channel);
		return ;
	}
	else if (it->second.getKMode() == true)
	{
		if (passwrd == it->second.getChannelPass())
			it->second.newClient(passwrd, client);
		else
		{
			Utils::sendFormattedMessage(ERR_BADCHANNELKEY, client, channel);
			return ;
		}
	}
	else
		it->second.newClient(passwrd, client);
	Utils::joinMessageSuccessful(client, server, channel);
}



void	Server::createOrJoinChannel(std::vector<std::string> channels, std::vector<std::string> passwrds, Client& client, Server& server)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != PREFIX_CHAN)
		{
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, channels[i]);
		}
		else if (_channelMap.find(channels[i]) == _channelMap.end())
		{
			createNewChannel(channels[i], client.getClientSocket(), server);
		}
		else if ( _channelMap.find(channels[i])->second.getIMode() == false)
		{
			if (i >= passwrds.size())
			{
				addClientToChannel(channels[i], "", client, server);
			}
			else
			{
				addClientToChannel(channels[i], passwrds[i], client, server);
			}
		}
		else
			Utils::sendFormattedMessage(ERR_INVITEONLYCHAN, client, channels[i]);
	}
}



bool	Server::isPartOfChannel(std::string channel_name, const Client& client)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(channel_name);

	if (it == _channelMap.end())
	{
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
		return (false);
	}
	std::vector<channelClient>	clients = it->second.getChannelClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].client->getClientNickname() == client.getClientNickname())
			return (true);
	}
	Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, channel_name);
	return (false);
}



void	Server::sendMessageToReceivers(std::vector<std::string> receivers, std::string message, const Client& client)
{
	for (size_t i = 0; i < receivers.size(); i++)
	{
		message = ":" + client.getClientNickname()
					+ "!~" + client.getClientUsername() + "@"
					+ client.getClientIP() + " PRIVMSG " + receivers[i]
					+ " :" + message;
		if (receivers[i][0] == '#' && isPartOfChannel(receivers[i], client) == true)
		{
			sendMessageToChannelNotSelf(receivers[i], message, client);
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
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
		return ;
	}
	std::string	full_message = message + END_MSG;

	// it->second.sendPrivmsgToChan(client, full_message);
	it->second.sendMessageToAll(full_message);
}

void	Server::sendMessageToChannelNotSelf(std::string receiver, std::string message, const Client& client)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(receiver);

	if (it == _channelMap.end())
	{
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
		return ;
	}
	std::string	full_message = message + END_MSG;

	it->second.sendPrivmsgToChan(client, full_message);
	// it->second.sendMessageToAll(full_message);
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
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
		return ;
	}
	std::string	full_message = message + END_MSG;

	Utils::sendMessage(full_message, it->second);
}



void	Server::removeClientsFromChannels(Client& client, std::vector<std::string> channels, std::vector<std::string> clients, std::string message)
{
	std::map<std::string, Channel>::iterator	it_channels;

	for (size_t pos = 0; pos < channels.size(); pos++)
	{
		it_channels = _channelMap.find(channels[pos]);
		if (it_channels != _channelMap.end())
		{
			it_channels->second.kickThoseMfOut(client, *this, clients, message);
		}
		else
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, channels[pos]);
	}
}

void Server::removeClientFromAllItsChan(Client& client)
{
	std::map<std::string, Channel>::iterator	it_channels;
	std::vector<std::map<std::string, Channel>::iterator>	chan_to_delete;

	for (it_channels = _channelMap.begin(); it_channels != _channelMap.end(); it_channels++)
	{
		if (it_channels->second.clientIsInChan(client) == true)
		{
			it_channels->second.removeClient(client);
			if (it_channels->second.getChannelClients().empty() == true)
				chan_to_delete.push_back(it_channels);
		}
	}
	for (size_t pos = 0; pos < chan_to_delete.size(); pos++)
	{
		_channelMap.erase(chan_to_delete[pos]);
	}
}



static std::string	modeStatusAfterExec(std::vector<std::string> modes_args, std::vector<std::string> modes_with_args, std::vector<std::string> modes_without_args)
{
	std::string	added_modes = "+";
	std::string	removed_modes = "-";
	std::string	parameters;

	for (size_t i = 0; i < modes_with_args.size(); i++)
	{
		std::string	mode(1, modes_with_args[i][1]);

		if (modes_with_args[i][0] == '+')
			added_modes += mode;
		else
			removed_modes += mode;
	}
	for (size_t i = 0; i < modes_without_args.size(); i++)
	{
		std::string	mode(1, modes_without_args[i][1]);

		if (modes_without_args[i][0] == '+')
			added_modes += mode;
		else
			removed_modes += mode;
	}
	for (size_t i = 0; i < modes_with_args.size(); i++)
	{
		if (i < modes_args.size())
			parameters += modes_args[i];
		if (i < modes_with_args.size() - 1)
			parameters += " ";
	}
	std::string	args;

	if (added_modes.size() > 1)
		args += added_modes + " " + parameters + " ";
	if (removed_modes.size() > 1)
		args += removed_modes;
	return (args);
}

static std::string	chanModes(const Channel &channel)
{
	std::string modes_of_chan = "";

	if (channel.getIMode() == true)
		modes_of_chan += 'i';
	if (channel.getTMode() == true)
		modes_of_chan += 't';
	if (channel.getKMode() == true)
		modes_of_chan += 'k';
	if (channel.getLMode() == true)
		modes_of_chan += 'l';
	return (modes_of_chan);
}

void	Server::displayChanModes(Client &client, const std::vector<std::string> channels)
{
	std::map<std::string, Channel>::iterator	it_channels;

	for (size_t pos = 0; pos < channels.size(); pos++)
	{
		it_channels = _channelMap.find(channels[pos]);
		if (it_channels != _channelMap.end())
		{
			client.setLastArgument(chanModes(it_channels->second));
			Utils::sendFormattedMessage(RPL_CHANNELMODES, client, channels[pos]);
		}
		else
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, channels[pos]);
	}
}



void	Server::changeChannelsModes(Client& client, std::vector<std::string> channels, std::vector<std::string> modes_args, std::vector<std::string> modes_with_args, std::vector<std::string> modes_without_args)
{
	std::map<std::string, Channel>::iterator	it_channels;

	for (size_t pos = 0; pos < channels.size(); pos++)
	{
		it_channels = _channelMap.find(channels[pos]);
		if (it_channels != _channelMap.end())
		{
			if (it_channels->second.isChanOp(client) == true)
			{
				it_channels->second.setSimpleModes(modes_without_args);
				it_channels->second.setArgModes(client, modes_args, modes_with_args);
				client.setLastArgument(modeStatusAfterExec(modes_args, modes_with_args, modes_without_args));
				Utils::sendFormattedMessage(RPL_CHANNELMODEIS, client, it_channels->second.getChannelName());
			}
			else
				Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, channels[pos]);
		}
		else if (pos < channels.size())
		{
			Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, channels[pos]);
		}
	}
}



void	Server::deleteChannel(std::string channel_name)
{
	std::map<std::string, Channel>::iterator	it = _channelMap.find(channel_name);

	_channelMap.erase(it);
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

const char *Server::BadPasswordException::what() const throw() { return (ERR_PASSWDMISMATCH); }

const char *Server::ClientQuitException::what() const throw() { return (ERR_PASSWDMISMATCH); }

// const char *Server::ReadException::what() const throw()
// {
// 	return ("\033[0;31mError: Could not read client's message.\n\033[0m");
// }



/*************************************/
/*************** PRIVATE *************/
/*************************************/
