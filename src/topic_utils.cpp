#include "../inc/Commands.hpp"


static void		changeChannelTopic(const Client& client, const std::string& channelname, const std::string& newtopic)
{
	std::vector<Channel*>					channels = client.getClientChannels();
	std::vector<Channel*>::const_iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannelName() == channelname)
		{
			(*it)->setChannelTopic(newtopic);
			Utils::sendErrorMessage(RPL_TOPIC, client, channelname);
			(*it)->sendMessageToAll(channelname + ": Topic changed to " + newtopic + '\n');
		}
	}
}



static std::string		getChannelTopic(const std::string& channelname, const Client& client)
{
	std::vector<Channel*>					channels = client.getClientChannels();
	std::vector<Channel*>::const_iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannelName() == channelname)
		{
			return ((*it)->getChannelTopic());
		}
	}
	return (EMPTY);
}


static void		displayTopic(const std::vector<std::string> parameters, const Client& client, const std::string& topic)
{
	if (topic == EMPTY)
	{
		Utils::sendErrorMessage(RPL_NOTOPIC, client, parameters[0]);
		return ;
	}

	Utils::sendErrorMessage("Topic :" + topic, client, parameters[0]);
}


void		Commands::chooseAndExecuteTopicAction(const std::vector<std::string> parameters, const Client& client)
{
	if (parameters.size() == 1)
	{
		const std::string		topic = getChannelTopic(parameters[0], client);
		displayTopic(parameters, client, topic);
	}

	if (parameters.size() == 2)
	{
		changeChannelTopic(client, parameters[0], parameters[1]);
	}
}



bool		Commands::areValidTopicParameters(const std::vector<std::string> parameters, const Client& client, const Server& server)
{
	if (Utils::channelExists(server, parameters[0]) == false)
	{
		Utils::sendErrorMessage(ERR_NOSUCHCHANNEL, client, parameters[0]);
		return (false);
	}

	if (client.userIsInChannel(parameters[0], client.getClientNickname()) == false)
	{
		Utils::sendErrorMessage(ERR_NOTONCHANNEL, client, parameters[0]);
		return (false);
	}

	if (client.isOperator(parameters[0]) == false)
	{
		Utils::sendErrorMessage(ERR_CHANOPRIVSNEEDED, client, parameters[0]);
		return (false);
	}
	return (true);
}
