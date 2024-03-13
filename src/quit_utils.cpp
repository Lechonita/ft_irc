#include "../inc/Utils.hpp"


void	Utils::notifyQuitinChannels(const Client& client, const Server& server)
{
	std::map<std::string, Channel>::iterator	itChannels;
	for (itChannels = server.getChannelMap().begin(); itChannels != server.getChannelMap().end(); ++itChannels)
	{
		std::vector<channelClient>::iterator	itClients;
		for (itClients = itChannels->second.getChannelClients().begin(); itClients != itChannels->second.getChannelClients().end(); ++itClients)
		{
			if (itClients->client->getClientSocket() == client.getClientSocket())
			{
				Utils::sendQuitMessagetoChannels(client.getClientNickname(), itChannels->second);
			}
		}
	}
}



void	Utils::sendQuitMessagetoChannels(const std::string& nickname, const Channel channel)
{
	std::vector<channelClient>::iterator	it = channel.getChannelClients().begin();
	for(; it != channel.getChannelClients().end(); ++it)
	{
		displayQuitChannelMessage(nickname, it->client->getClientSocket());
	}
}



static std::string		quitMessage(const std::string& nickname)
{
	return (nickname + LEFT_CHANNEL);
}


void	Utils::displayQuitChannelMessage(const std::string& nickname, const int receiverSocket)
{
	if(send(receiverSocket, quitMessage(nickname).c_str(), strlen(quitMessage(nickname).c_str()), 0) == ERROR)
	{
		std::perror(PERR_SEND);
	}
	else
	{
		std::cout << ORANGE << nickname << LEFT_CHANNEL << NC;
	}
}
