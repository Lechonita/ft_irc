#include "../inc/Utils.hpp"


void	Utils::notifyQuitinChannels(const Client& client, const Server& server)
{
	// printf("Arrivée dans 1/ notifyQuitinChannels\n");

	std::map<std::string, Channel>::iterator	itChannels;
	for (itChannels = server.getChannelMap().begin(); itChannels != server.getChannelMap().end(); ++itChannels)
	{
		// printf("  Entrée dans la première boucle for\n");
		std::vector<channelClient>::iterator	itClients;
		for (itClients = itChannels->second.getChannelClients().begin(); itClients != itChannels->second.getChannelClients().end(); ++itClients)
		{
			// printf("    Entrée dans la deuxième boucle for\n");
			int		currentSocket = itClients->client->getClientSocket();
			if (currentSocket == client.getClientSocket())
			{
				// printf("      Entrée dans if\n");
				Utils::sendQuitMessagetoChannels(client.getClientNickname(), itChannels->second);
			}
		}
	}
}



void	Utils::sendQuitMessagetoChannels(const std::string& nickname, const Channel channel)
{
	// printf("Entrée dans sendQuitMessagetoChannels\n");

	std::vector<channelClient>::iterator	it; // issue here
	for(it = channel.getChannelClients().begin(); it != channel.getChannelClients().end(); ++it)
	{
		displayQuitChannelMessage(nickname, it->client->getClientSocket());
	}
}



static std::string		quitMessage(const std::string& nickname)
{
	// printf("Entrée dans quitMessage\n");

	return (nickname + LEFT_CHANNEL);
}


void	Utils::displayQuitChannelMessage(const std::string& nickname, const int receiverSocket)
{
	// printf("Entrée dans displayQuitChannelMessage\n");

	if(send(receiverSocket, quitMessage(nickname).c_str(), strlen(quitMessage(nickname).c_str()), 0) == ERROR)
	{
		std::perror(PERR_SEND);
	}
	else
	{
		std::cout << ORANGE << nickname << LEFT_CHANNEL << NC;
	}
}
