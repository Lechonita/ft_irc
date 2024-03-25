#include "../inc/Commands.hpp"


// Constructor

Commands::Commands()
{
	_cmdMap["JOIN"] = &Commands::commandJOIN;
	_cmdMap["PASS"] = &Commands::commandPASS;
	_cmdMap["NICK"] = &Commands::commandNICK;
	_cmdMap["USER"] = &Commands::commandUSER;
	_cmdMap["CAP"] = &Commands::commandCAP;
	_cmdMap["QUIT"] = &Commands::commandQUIT;
	_cmdMap["PRIVMSG"] = &Commands::commandPRIVMSG;
	_cmdMap["PART"] = &Commands::commandPART;
	_cmdMap["INVITE"] = &Commands::commandINVITE;
	_cmdMap["PING"] = &Commands::commandPING;
	_cmdMap["TOPIC"] = &Commands::commandTOPIC;
	_cmdMap["WHOIS"] = &Commands::commandWHOIS;
}


// JOIN


void		Commands::commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientUsername(), client, USER_NOT_ENTERED) == false)
			return ;
	}

	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;
	std::map<std::string, std::string*>	chan_pass;

	std::string	join_params = eraseCommandfromLine(line, command);
	if (join_params.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkJoinParams(join_params, &channels, &passwrds);
	server.createOrJoinChannel(channels, passwrds, client, server);
}



//PART


void		Commands::commandPART(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)server;

	if (client.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientUsername(), client, USER_NOT_ENTERED) == false)
			return ;
	}

	std::string					message;
	std::vector<std::string>	channels;

	std::string	part_params = eraseCommandfromLine(line, command);
	if (part_params.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkPartParams(part_params, &channels, &message);
	client.partFromChannels(client, server, channels, message);
}



//PRIVMSG



void		Commands::commandPRIVMSG(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
			return ;
		if (isParameterSetUp(client.getClientUsername(), client, USER_NOT_ENTERED) == false)
			return ;
	}

	std::string					message;
	std::vector<std::string>	receivers;

	std::string	privmsg_params = eraseCommandfromLine(line, command);
	if (privmsg_params.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkPrivmsgParams(privmsg_params, &receivers, &message);
	if (message.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NOTEXTTOSEND, client);
		return ;
	}
	server.sendMessageToReceivers(receivers, message, client);
}



// PASS

void		Commands::commandPASS(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientPassword() != EMPTY)
	{
		Utils::sendFormattedMessage(ERR_ALREADYREGISTERED, client);
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);

	if (isParameterSetUp(password, client, ERR_NEEDMOREPARAMS) == false)
		return ;

	if (isValidPassword(password, client, server) == true)
	{
		client.setPassword(password);
		client.setLastArgument(password);
	}
}



// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;
	}

	const std::string	nickname = eraseCommandfromLine(line, command);
	if (nickname.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	client.setLastArgument(nickname);

	if (isParameterSetUp(nickname, client, EMPTY) == false)
		return ;

	if (isValidNickname(nickname, client, server) == true)
	{
		client.setNickname(nickname);
	}
}



// USER

void		Commands::commandUSER(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)server;

	if (client.getIrssi() == false)
	{
		if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
			return ;

		if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
			return ;
	}

	const std::string	userInfo = eraseCommandfromLine(line, command);
	if (userInfo.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	client.setLastArgument(userInfo);

	if (isParameterSetUp(userInfo, client, EMPTY) == false)
		return ;


	const std::vector<std::string>	parameters = Utils::splitParameters(userInfo);

	if (areValidUserParameters(parameters) == false)
	{
		Utils::sendFormattedMessage(USER_PARAM_KO, client);
		return ;
	}

	client.setUsername(parameters[0]);
	client.setRealName(parameters);
	client.setClientStatus(CONNECTED);
	Utils::displayWelcomeMessage(client);
}



// CAP

void		Commands::commandCAP(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)server;

	const std::string	parameter = eraseCommandfromLine(line, command);
	client.setLastArgument(parameter);

	if (isParameterSetUp(parameter, client, EMPTY) == false)
		return ;

	if (isIrssi(parameter) == true)
		client.setIrssi(true);
}



// QUIT

void		Commands::commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server)
{
	// if (client.getClientStatus() == DISCONNECTED)
	// {
	// 	Utils::sendFormattedMessage(NOT_CONNECTED, client);
	// 	return ;
	// }

	(void)command;
	client.setLastArgument(line);

	close(client.getClientSocket());

	Utils::notifyQuitinChannels(client, server);

	server.removeClientfromServer(client);
	server.removeClientfromChannels(client);




	// Retirer le client du server :
		// 1. std::map<int, Client>				_clientMap
		// 2. std::map<std::string, Channel>	_channelMap
	
	// Retirer le client d'un channel :
		// std::vector<channelClient>			_channelClients
		// delete le channel si c'était le dernier client

	// quand un client quit, il faut que ca quitte le programme mais que pour lui
}



void		Commands::commandINVITE(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
	{
		Utils::sendFormattedMessage(NOT_CONNECTED, client);
		return ;
	}

	const std::string	invitation = eraseCommandfromLine(line, command);
	client.setLastArgument(invitation);

	const std::vector<std::string>	parameters = Utils::splitParameters(invitation);

	if (invitation.empty() == true || parameters.size() < 2)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	if (parameters.size() > 2)
	{
		Utils::sendFormattedMessage(TOO_MANY_PARAM, client);
		return ;
	}

	if (areValidInviteParameters(parameters, client, server) == false)
	{
		return ;
	}

	server.inviteUser(parameters, client);

	// INVITE <invitee> <channelname>
}



// PING

void		Commands::commandPING(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)server;
	const std::string	parameter = eraseCommandfromLine(line, command);
	if (parameter.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NOORIGIN, client);
		return ;
	}

	Utils::sendFormattedMessage(PONG, client);
}



// PING

void		Commands::commandTOPIC(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
	{
		Utils::sendFormattedMessage(NOT_CONNECTED, client);
		return ;
	}

	const std::string	arguments = eraseCommandfromLine(line, command);
	client.setLastArgument(arguments);

	if (arguments.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}

	const std::vector<std::string>	parameters = Utils::splitParameters(arguments);

	if (areValidTopicParameters(parameters, client, server) == false)
	{
		return ;
	}

	chooseAndExecuteTopicAction(parameters, client);
}

// /dcc <nom de la personne> send <fichier>


// WHOIS

void		Commands::commandWHOIS(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientStatus() == DISCONNECTED)
	{
		Utils::sendFormattedMessage(NOT_CONNECTED, client);
		return ;
	}

	const std::string	argument = eraseCommandfromLine(line, command);
	client.setLastArgument(argument);

	if (argument.empty() == true)
	{
		Utils::sendFormattedMessage(ERR_NONICKNAMEGIVEN, client);
		return ;
	}

	const std::vector<std::string>	parameters = Utils::splitParameters(argument);

	switch (getParameterType(parameters[0], server))
	{
		case USER_TYPE:
			server.sendUserInformation(parameters, client);
			break ;
		
		case CHANNEL_TYPE:
			server.sendChannelInformation(parameters, client);
			break ;

		default:
			if (parameters[0].at(0) == PREFIX_CHAN)
				Utils::sendFormattedMessage(ERR_NOSUCHCHANNEL, client, parameters[0]);
			else
				Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
	}

	// ERR_NOSUCHSERVER
	// RPL_WHOISUSER
	// RPL_WHOISCHANNELS
	// RPL_WHOISSERVER
	// RPL_AWAY
	// RPL_WHOISOPERATOR
	// RPL_WHOISIDLE
	// RPL_ENDOFWHOIS

}