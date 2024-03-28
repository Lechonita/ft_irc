#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/defines.hpp"
#include "../inc/Commands.hpp"


// void	Server::interpretClientBuffer(const std::map<int, Client>::iterator &client, Server& server)
// {
// 	client->second.interpretMessage(server);
// }

void	Server::interpretMessage(const std::map<int, Client>::iterator &client)
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
			Commands::findCommandInMessage(line, *this, client->second);
		//il faudrait que cette fonction ne soit pas dans le client et le probleme est regle, on est dans un client qui n'existe plus et _buffer n'existe plus
		_buffer.erase(0, _buffer.find("\n") + 1);
		pos = _buffer.find("\r\n");
		if (pos == std::string::npos)
			pos = _buffer.find("\n");
	}
}

void	Server::setClientBuffer(const char *buffer)
{
	_buffer += buffer;
	// client->second.setBuffer(buffer);
}



void	Server::manageClientMessageReception(const char *buffer, const int& clientSocket)
{
	std::map<int, Client>::iterator	it;
	for (it = _clientMap.begin(); it != _clientMap.end(); ++it)
	{
		if (it->first == clientSocket)
			break ;
	}

	if (it != _clientMap.end())
	{
		setClientBuffer(buffer);
		// interpretClientBuffer(it, *this);
		interpretMessage(it);
	}
}
