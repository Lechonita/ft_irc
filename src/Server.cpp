#include "../inc/Server.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Server::Server(const std::string &port, const std::string &password)
	: _port(port),
	  _password(password),
	  _nbClients(0),
	  _pollFd(0)
	//   _cmdList(setCommandList())
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
	_clientMap.clear();
	// std::cout << "Default server destructor." << std::endl;
}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

/*************************************/
/*************** PUBLIC **************/
/*************************************/

// Debug

static void	printClientMap(const std::map<int, Client>  &clientMap)
{
	std::map<int, Client>::const_iterator it;

	for (it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		std::cout << "ClientMap #" << it->first << " : " << it->second.getClientSocket() << std::endl;
	}
}


// Functions


void	Server::runServer()
{
	if (poll(&_pollFd[0], _pollFd.size(), TIMEOUT) == ERROR)
	{
		close(_serverSocket);
		throw(PollException());
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

	std::cout << "New connection - Client fd = " << clientSocket
			<< " / ip = " << inet_ntoa(clientAddress.sin_addr)
			<< " / port = " << ntohs(clientAddress.sin_port) << std::endl;

	// Create the new client object and store it in std::map
	Client	newClient(clientSocket);
	_clientMap.insert(std::make_pair(clientSocket, newClient));
	_nbClients += 1;

	_pollFd.push_back(pollfd());
	_pollFd.back().fd = clientSocket;
	_pollFd.back().events = POLLIN; // We want to monitor data reception on this file descriptor

	printClientMap(_clientMap);
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
				manageClientMessageReception(buffer, clientSocket);
				memset(buffer, 0, BUFFERSIZE);
			}

			// Read chunks of data until there is no more data to read
			// while (bytesRead > 0)
			// {
			// 	std::cout << std::string(buffer, bytesRead) << std::endl;
			// 	memset(buffer, 0, BUFFERSIZE);
			// 	bytesRead = recv(clientSocket, buffer, BUFFERSIZE, 0);
			// }
		}
	}
}



// Getters

int							Server::getSocketFd() const { return (_serverSocket); }

std::string					Server::getPassword() const { return (_password); }

// std::vector<std::string>	Server::getCommandList() const { return (_cmdList); }



// Setters

// std::vector<std::string>	Server::setCommandList()
// {
// 	std::vector<std::string>	res;

// 	res.push_back("JOIN");
// 	res.push_back("PASS");
// 	return (res);
// }


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

// const char *Server::ReadException::what() const throw()
// {
// 	return ("\033[0;31mError: Could not read client's message.\n\033[0m");
// }



/*************************************/
/*************** PRIVATE *************/
/*************************************/
