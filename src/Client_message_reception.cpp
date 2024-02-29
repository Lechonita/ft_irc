#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/defines.hpp"



void	Server::interpretClientBuffer(const std::map<int, Client>::iterator &client, const Server& server)
{
	client->second.interpretMessage(server);
}



void	Server::setClientBuffer(const std::map<int, Client>::iterator &client, const char *buffer)
{
	client->second.setBuffer(buffer);
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
		setClientBuffer(it, buffer);
		interpretClientBuffer(it, *this);
	}
}
