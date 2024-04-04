#include "../inc/Commands.hpp"


static void		changeChannelTopic(const Client& client, const std::string& channelname, const std::string& newtopic)
{
	std::vector<Channel*>					channels = client.getClientChannels();
	std::vector<Channel*>::const_iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannelName() == channelname)
		{
			if ((*it)->getTMode() == false || ((*it)->getTMode() == true && (*it)->isChanOp(client) == true))
			{
				(*it)->setChannelTopic(newtopic);
				std::string	message = ":" + client.getClientNickname() + "!" + client.getClientUsername() + RPL_TOPIC;
				(*it)->sendMessageToAll(message, channelname);
			}
			else if ((*it)->getTMode() == true && (*it)->isChanOp(client) == false)
				Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, channelname);
		}
	}
}



static void		displayTopic(const std::vector<std::string> parameters, const Client& client, const std::string& topic)
{
	if (topic == EMPTY)
	{
		Utils::sendFormattedMessage(RPL_NOTOPIC, client, parameters[0]);
		return ;
	}

	Utils::sendFormattedMessage("Topic :" + topic, client, parameters[0]);
}


void		Commands::chooseAndExecuteTopicAction(std::vector<std::string> parameters, const Client& client)
{
	if (parameters.size() == 1)
	{
		const std::string		topic = Utils::getChannelTopic(parameters[0], client);
		displayTopic(parameters, client, topic);
	}
	else if (parameters.size() == 2)
	{
		if (parameters[1][0] == ':')
			parameters[1].erase(0, 1);
		changeChannelTopic(client, parameters[0], parameters[1]);
	}
}



bool		Commands::areValidTopicParameters(const std::vector<std::string> parameters, const Client& client, const Server& server)
{
	if (Utils::channelExists(server, parameters[0]) == false)
	{
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, parameters[0]);
		return (false);
	}

	if (client.userIsInChannel(parameters[0], client.getClientNickname()) == false)
	{
		Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, parameters[0]);
		return (false);
	}
	return (true);
}
