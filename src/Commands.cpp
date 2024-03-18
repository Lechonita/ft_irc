#include "../inc/Commands.hpp"


// Constructor

Commands::Commands()
{
	_cmdList["JOIN"] = &Commands::commandJOIN;
	_cmdList["PASS"] = &Commands::commandPASS;
	_cmdList["NICK"] = &Commands::commandNICK;
	_cmdList["USER"] = &Commands::commandUSER;
	_cmdList["CAP"] = &Commands::commandCAP;
	_cmdList["QUIT"] = &Commands::commandQUIT;
}


// JOIN


void		Commands::commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server)
{
	std::string					join_params;
	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;
	std::map<std::string, std::string*>	chan_pass;

	join_params = eraseCommandfromLine(line, command);
	if (join_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkJoinParams(join_params, &channels, &passwrds);

	server.manageChannel(channels, passwrds, client);
}



//PART


void		Commands::commandPART(const std::string& line, const std::string& command, Server& server, Client& client)
{
	std::string					part_params;
	std::vector<std::string>	channels;

	part_params = eraseCommandfromLine(line, command);
	if (part_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, client);
		return ;
	}
	checkPartParams(part_params, &channels);

	for (size_t i = 0; i < channels.size(); i++)
		std::cout << channels[i] << std::endl;
	server.partFromChannels(client, channels);
}



//PRIVMSG



void		Commands::commandPRIVMSG(const std::string& line, const std::string& command, Server& server, Client& client)
{
	std::string					privmsg_params;
	std::string					message;
	std::vector<std::string>	receivers;

	(void)server;
	privmsg_params = eraseCommandfromLine(line, command);
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

	if (isParameterSetUp(nickname, client, EMPTY) == false)
		return ;

	if (isValidNickname(nickname, server) == true)
	{
		client.setNickname(nickname, server.getIrssi());
		client.setLastArgument(nickname);
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

	if (isParameterSetUp(userInfo, client, EMPTY) == false)
		return ;


	const std::vector<std::string>	parameters = Utils::splitParameters(userInfo);

	if (areValidUserParameters(parameters) == false)
		return ;

	client.setUsername(parameters[0], server.getIrssi());
	client.setLastArgument(userInfo);
	Utils::displayWelcomeMessage(client);
}



// CAP

void		Commands::commandCAP(const std::string& line, const std::string& command, Client& client, Server& server)
{
	const std::string	parameter = eraseCommandfromLine(line, command);

	if (isParameterSetUp(parameter, client, EMPTY) == false)
		return ;

	if (isIrssi(parameter) == true)
		server.setIrssi(true);
}



// QUIT

void		Commands::commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server)
{
	(void)command;
	client.setLastArgument(line);

	close(client.getClientSocket());

	Utils::notifyQuitinChannels(client, server);

	server.removeClientfromServer(client);

	// remove client from channel list (channel) >> send message to this user's channels to norify other users
	// if it was last client from a channel, delete that channel
	// other memory allocations
}