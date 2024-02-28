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
	std::cout << "Default server destructor." << std::endl;
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

void Server::runServer()
{
	if (poll(&_pollFd[0], _pollFd.size(), TIMEOUT) == ERROR)
	{
		close(_serverSocket);
		throw(PollException());
	}

	if (_pollFd[0].revents == POLLIN)
	{
		try {
			createNewClient();
		}
		catch (Server::Exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
	// else
	// {
	// 	getClientMessage();
	// }
}


void Server::createNewClient()
{
	size_t clientIndex = 0;
	std::map<int, Client>::iterator it;
	for (it = _clientMap.begin(); it != _clientMap.end(); ++it)
	{
		++clientIndex;
	}

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
	Client newClient(clientSocket);
	_clientMap.insert(std::make_pair(clientIndex, newClient));
	_nbClients += 1;

	_pollFd.push_back(pollfd());
	_pollFd.back().fd = clientSocket;
	_pollFd.back().events = POLLIN; // We want to monitor data reception on this file descriptor

	printClientMap(_clientMap);
}


// Getters

int Server::getSocketFd() const { return (_serverSocket); }

std::string Server::getPassword() const { return (_password); }


// Exceptions

const char *Server::Exception::what() const throw()
{
	return ("\033[0;31mDefault Server Exception\n\033[0m");
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

const char *Server::PollException::what() const throw()
{
	return ("\033[0;31mError: Bad file descriptor.\n\033[0m");
}

const char *Server::BlockException::what() const throw()
{
	return ("\033[0;31mError: Could not set server I/O operations to non-blocking.\n\033[0m");
}

const char *Server::AcceptException::what() const throw()
{
	return ("\033[0;31mError: Could not connect new client.\n\033[0m");
}


/*************************************/
/*************** PRIVATE *************/
/*************************************/
