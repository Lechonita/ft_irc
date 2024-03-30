#include "../inc/Server.hpp"
#include "../inc/defines.hpp"

void	Server::removeClientFromMap(const int& clientSocket)
{
	std::map<int, Client>::iterator	it;
	for (it = _clientMap.begin(); it != _clientMap.end(); ++it)
	{
		if (it->second.getClientSocket() == clientSocket)
			break;
	}
	if (it != _clientMap.end())
		_clientMap.erase(it);
}

void	Server::removeClientFromPollFd(const int& clientSocket)
{
	std::vector<pollfd>	::iterator	it;
	for (it = _pollFd.begin(); it != _pollFd.end(); ++it)
	{
		if (it->fd == clientSocket)
			break;
	}
	if (it != _pollFd.end())
		_pollFd.erase(it);
}

void	Server::disconnectClient(const int& clientSocket)
{
	removeClientFromPollFd(clientSocket);
	removeClientFromMap(clientSocket);
	close(clientSocket);

	_nbClients -= 1;
	// if (_nbClients < 1)
	// 	_clientMap.clear();
}
