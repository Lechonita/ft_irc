#include "../inc/Commands.hpp"


// Constructor

Commands::Commands()
{
	_cmdMap["JOIN"] = &Commands::commandJOIN;
	_cmdMap["PASS"] = &Commands::commandPASS;
	_cmdMap["MODE"] = &Commands::commandMODE;
	_cmdMap["NICK"] = &Commands::commandNICK;
	_cmdMap["USER"] = &Commands::commandUSER;
	_cmdMap["CAP"] = &Commands::commandCAP;
	_cmdMap["QUIT"] = &Commands::commandQUIT;
	_cmdMap["PRIVMSG"] = &Commands::commandPRIVMSG;
	_cmdMap["PART"] = &Commands::commandPART;
	_cmdMap["KICK"] = &Commands::commandKICK;
	_cmdMap["INVITE"] = &Commands::commandINVITE;
	_cmdMap["PING"] = &Commands::commandPING;
}


// JOIN


void		Commands::commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
		return ;

	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;
	std::map<std::string, std::string*>	chan_pass;

	std::string	join_params = eraseCommandfromLine(line, command);
	if (join_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkJoinParams(join_params, &channels, &passwrds);
	server.createOrJoinChannel(channels, passwrds, client, server);
}



//KICK

void		Commands::commandKICK(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
		return ;

	std::string	kick_params = eraseCommandfromLine(line, command);
	if (kick_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	std::string					message;
	std::vector<std::string>	channels;
	std::vector<std::string>	clients;

	checkKickParams(kick_params, &channels, &clients, &message);
	server.removeClientsFromChannels(client, channels, clients, message);
}



//PART


void		Commands::commandPART(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
		return ;

	std::string					message;
	std::vector<std::string>	channels;

	std::string	part_params = eraseCommandfromLine(line, command);
	if (part_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkPartParams(part_params, &channels, &message);
	client.partFromChannels(client, server, channels, message);
}



void	Commands::commandMODE(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
		return ;

	std::vector<std::string>			channels;
	std::vector<std::string>			modes_args;
	std::vector<std::string>			modes_with_args;
	std::vector<std::string>			modes_without_args;
	std::string							mode_params = eraseCommandfromLine(line, command);

	(void)server;
	if (mode_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkModeParams(mode_params, &channels, &modes_with_args, &modes_without_args, &modes_args, client);
	for (size_t pos = 0; pos < channels.size(); pos++)
	{

	}
	server.changeChannelsModes(client, channels, modes_args, modes_with_args, modes_without_args);

}



//PRIVMSG



void		Commands::commandPRIVMSG(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
		return ;

	std::string					message;
	std::vector<std::string>	receivers;

	std::string	privmsg_params = eraseCommandfromLine(line, command);
	if (privmsg_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkPrivmsgParams(privmsg_params, &receivers, &message);
	if (message.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NOTEXTTOSEND, client);
		return ;
	}
	server.sendMessageToReceivers(receivers, message, client);
}



// PASS

void		Commands::commandPASS(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientPassword() != EMPTY)
	{
		Utils::sendErrorMessage(ERR_ALREADYREGISTERED, client);
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);

	if (isParameterSetUp(password, client, ERR_NEEDMOREPARAMS) == false)
		return ;

	if (isValidPassword(password, client, server) == true)
	{
		client.setPassword(password, server.getIrssi());
		client.setLastArgument(password);
	}
}



// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (server.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;
	}

	const std::string	nickname = eraseCommandfromLine(line, command);
	if (nickname.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	client.setLastArgument(nickname);

	if (isParameterSetUp(nickname, client, EMPTY) == false)
		return ;

	if (isValidNickname(nickname, client, server) == true)
	{
		client.setNickname(nickname, server.getIrssi());
	}
}



// USER

void		Commands::commandUSER(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (server.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;

		if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
			return ;
	}

	const std::string	userInfo = eraseCommandfromLine(line, command);
	if (userInfo.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	client.setLastArgument(userInfo);

	if (isParameterSetUp(userInfo, client, EMPTY) == false)
		return ;


	const std::vector<std::string>	parameters = Utils::splitParameters(userInfo);

	if (areValidUserParameters(parameters) == false)
	{
		Utils::sendErrorMessage(USER_PARAM_KO, client);
		return ;
	}

	client.setUsername(parameters[0], server.getIrssi());
	client.setClientStatus(CONNECTED);
	Utils::displayWelcomeMessage(client);
}



// CAP

void		Commands::commandCAP(const std::string& line, const std::string& command, Client& client, Server& server)
{
	const std::string	parameter = eraseCommandfromLine(line, command);
	client.setLastArgument(parameter);

	if (isParameterSetUp(parameter, client, EMPTY) == false)
		return ;

	if (isIrssi(parameter) == true)
		server.setIrssi(true);
}



// QUIT

void		Commands::commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
	{
		Utils::sendErrorMessage(NOT_CONNECTED, client);
		return ;
	}

	(void)command;
	client.setLastArgument(line);

	close(client.getClientSocket());

	Utils::notifyQuitinChannels(client, server);

	server.removeClientfromServer(client);

	// remove client from channel list (channel) >> send message to this user's channels to norify other users
	// if it was last client from a channel, delete that channel
	// other memory allocations
}



void		Commands::commandINVITE(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
	{
		Utils::sendErrorMessage(NOT_CONNECTED, client);
		return ;
	}

	const std::string	invitation = eraseCommandfromLine(line, command);
	const std::vector<std::string>	parameters = Utils::splitParameters(invitation);

	if (invitation.empty() == true || parameters.size() < 2)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	if (parameters.size() > 2)
	{
		Utils::sendErrorMessage(TOO_MANY_PARAM, client);
		return ;
	}

	if (areValidInviteParameters(parameters, client, server) == false)
	{
		return ;
	}

	inviteUser(parameters, client, server);

	// INVITE <invitee> <channelname>
}



// PING

void		Commands::commandPING(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)server;
	const std::string	parameter = eraseCommandfromLine(line, command);
	if (parameter.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NOORIGIN, client);
		return ;
	}

	Utils::sendErrorMessage(PONG, client);
}
