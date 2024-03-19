#include "../inc/Commands.hpp"


bool		Commands::areValidInviteParameters(const std::vector<std::string> parameters, const Client& client, const Server& server)
{
	if (channelExists(server, parameters[1]) == false)
	{
		Utils::sendErrorMessage(ERR_NOSUCHCHANNEL, client, parameters[1]);
		return (false);
	}

	if (userIsInChannel(client, parameters[1], client.getClientNickname()) == false)
	{
		Utils::sendErrorMessage(ERR_NOTONCHANNEL, client, parameters[1]);
		return (false);
	}

	if (isOperator(client, parameters[1]) == false)
	{
		Utils::sendErrorMessage(ERR_CHANOPRIVSNEEDED, client, parameters[1]);
		return (false);
	}

	if (nicknameAlreadyExists(parameters[0], server) == false)
	{
		Utils::sendErrorMessage(ERR_NOSUCHNICK, client, parameters[1]);
		return (false);
	}

	if (userIsInChannel(client, parameters[1], parameters[0]) == true)
	{
		Utils::sendErrorMessage(ERR_USERONCHANNEL, client, parameters[1]);
		return (false);
	}
	return (true);
}



bool		Commands::userIsInChannel(const Client& client, const std::string& channelname, const std::string& usernickname)
{
	std::vector<Channel*>			channels = client.getClientChannels();
	std::vector<Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		std::vector<channelClient>				channelClients = (*it)->getChannelClients();
		std::vector<channelClient>::iterator	itClient;

		for (itClient = channelClients.begin(); itClient != channelClients.end(); ++itClient)
		{
			if ((*it)->getChannelName() == channelname && itClient->client->getClientNickname() == usernickname)
				return (true);
		}
	}
	return (false);


	// const std::string	clientList = Utils::getClientListInChannel(client, channelname);

	// if (clientList.find(usernickname) != std::string::npos)
	// 	return (true);
	// return (false);
}


static std::string		getChannelListInServer(const Server& server)
{
	std::map<std::string, Channel>	channels = server.getChannelMap();
	std::string						channel_list = "";

	std::map<std::string, Channel>::iterator	it;
	for (it = channels.begin(); it != channels.end(); ++it)
	{
		channel_list += it->second.getChannelName();
		channel_list += " ";
	}
	return (channel_list);
}


bool		Commands::channelExists(const Server& server, const std::string& channelname)
{
	const std::string	channelList = getChannelListInServer(server);

	if (channelList.find(channelname) != std::string::npos)
		return (true);
	return (false);
}


bool		Commands::isOperator(const Client& client, const std::string channelname)
{
	std::vector<Channel*>			channels = client.getClientChannels();
	std::vector<Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		std::vector<channelClient>				channelClients = (*it)->getChannelClients();
		std::vector<channelClient>::iterator	itClient;

		for (itClient = channelClients.begin(); itClient != channelClients.end(); ++itClient)
		{
			if ((*it)->getChannelName() == channelname && itClient->client->getClientNickname() == client.getClientNickname() && itClient->isOperator == true)
				return (true);
		}
	}
	return (false);
}

static Client*		findClient(const std::string& clientNickname, const Server& server)
{
	std::map<int, Client>	clientMap = server.getClientMap();
	std::map<int, Client>::iterator		it;

	for (it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		if (it->second.getClientNickname() == clientNickname)
			return (&it->second);
	}
	return (NULL);
}


// static Channel*		findChannel(const std::string& channelname, const Server& server)
// {
// 	std::map<std::string, Channel>				channelMap = server.getChannelMap();
// 	std::map<std::string, Channel>::iterator	it;

// 	for (it = channelMap.begin(); it != channelMap.end(); ++it)
// 	{
// 		if (it->second.getChannelName() == channelname)
// 			return (&it->second);
// 	}
// 	return (NULL);
// }


void		Commands::inviteUser(const std::vector<std::string> parameters, Client& client, Server& server)
{
	// send message to both RPL_INVITING
	Utils::sendErrorMessage(RPL_INVITING, client, parameters[1]);


	// add invitee in Channel's client list
	Client	*newClient = findClient(parameters[0], server);
	if (newClient != NULL)
	{
		server.addClientToChannel(parameters[1], newClient->getClientPassword(), *newClient); // message included

		// add channel in invitee's channel list
		// Channel	*channel = findChannel(parameters[1], server);
		// if (channel != NULL)
		// 	(*newClient).addChannelToClient(channel);
	}
	// le user nouvellement ajouté n'est pas proprement ajouté dans la liste des clients du channel
	// en attente
}
