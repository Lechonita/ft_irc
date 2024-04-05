#include "../inc/Utils.hpp"


void	Utils::notifyQuitinChannels(const Client& client, Server& server)
{
	(void)server;

	std::vector<Channel*>				channels = client.getClientChannels();
	std::vector<Channel*>::iterator		it;

	if (channels.size() > 0)
	{
		for(it = channels.begin(); it != channels.end(); ++it)
		{
			std::vector<channelClient>				clients = (*it)->getChannelClients();
			std::vector<channelClient>::iterator	itClient;

			for (itClient = clients.begin(); itClient != clients.end(); ++itClient)
			{
				if (itClient->client->getClientNickname() == client.getClientNickname())
				{
					(*it)->sendMessageToAll(RPL_QUIT, client.getClientNickname());
					break ;
				}
			}
		}
	}
}
