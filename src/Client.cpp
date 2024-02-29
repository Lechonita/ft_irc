#include "../inc/Client.hpp"
#include "../inc/defines.hpp"

/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Client::Client(const int& clientSocket): _clientSocket(clientSocket)
{
	_clientUsername = "";
	_clientPassword = "";
	_clientNickname = "";
	_buffer = "";

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

int		Client::getClientSocket() const { return (_clientSocket); }


// Setters

void	Client::setBuffer(const char *buffer) { _buffer += buffer; }


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
			parseClientMessage(line, server);

		_buffer.erase(0, _buffer.find("\n") + 1);
		pos = _buffer.find("\r\n");
		if (pos == std::string::npos)
			pos = _buffer.find("\n");
	}

	// if (!(_buffer.empty()))
	// {
	// 	char	command[10];
	// 	toupperBuff(command, _buffer.c_str());

	// 	// If it's a message from the client to the server
	// 	if (isalpha((unsigned char)*command))
	// 	{
	// 		handleCommandFromClientToServer();
	// 	}
	// 	else if (*command == ':') // It's a private message
	// 		handlePrivateMessage(&server);
	// 	else // It's a public message
	// 		sendPublicMessage(&server);
	// 	_buffer.clear();
	// }
}



void	Client::parseClientMessage(const std::string& line, const Server& server)
{
	const std::string		command = getCommandFromLine(line);

	if (command.empty() == true)
		return ;
	
	if (isCommandFromList(command, server) == false)
		return ;
	
	server.executeCommand(line, command);
}


std::string		Client::getCommandFromLine(const std::string& line) const
{
	std::string		command;
	
	for(size_t i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) == true)
			break ;
		command[i] = line[i];
	}
	return (command);
}


bool	Client::isCommandFromList(const std::string& command, const Server& server) const
{
	std::vector<std::string>::iterator	it;
	for(it = server.getCommandList().begin(); it != server.getCommandList().end(); ++it)
	{
		if ((*it).compare(command) == IS_EQUAL)
			return (true);
	}
	return (false);
}




// Exceptions

const char *Client::Exception::what() const throw() { return (ERR_CLIENT); }

const char *Client::BlockException::what() const throw() { return (ERR_CLIENT_BLOCK); }
