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

	// int		indexToRemove = -1;
	// for (size_t i = 0; i < _pollFd.size(); ++i)
	// {
	// 	if (_pollFd[i].fd == clientSocket)
	// 	{
	// 		indexToRemove = static_cast<int>(i);
	// 		break;
	// 	}
	// }

	// _pollFd.erase(_pollFd.begin() + indexToRemove);
}

void	Server::disconnectClient(const int& clientSocket)
{
	Utils::notifyQuitinChannels(_clientMap.find(clientSocket)->second, *this);
	removeClientFromAllItsChan(_clientMap.find(clientSocket)->second);
	_clientMap.find(clientSocket)->second.resetClientStatus(DISCONNECTED);
	removeClientFromMap(clientSocket);
	close(clientSocket);
	removeClientFromPollFd(clientSocket);
}
