#include "../inc/Commands.hpp"


bool		Commands::areValidInviteParameters(const std::vector<std::string> parameters, const Client& client, const Server& server)
{
	if (Utils::channelExists(server, parameters[1]) == false)
	{
		Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, parameters[1]);
		return (false);
	}

	if (client.userIsInChannel(parameters[1], client.getClientNickname()) == false)
	{
		Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, parameters[1]);
		return (false);
	}

	if (client.isOperator(parameters[1]) == false)
	{
		Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, parameters[1]);
		return (false);
	}

	if (nicknameAlreadyExists(parameters[0], client, server) == false)
	{
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client, parameters[1]);
		return (false);
	}

	if (client.userIsInChannel(parameters[1], parameters[0]) == true)
	{
		Utils::sendFormattedMessage(ERR_USERONCHANNEL, client, parameters[1]);
		return (false);
	}
	return (true);
}
