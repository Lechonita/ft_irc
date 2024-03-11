#include "../inc/Commands.hpp"


// JOIN


void		Commands::commandJOIN(const std::string& line, const std::string& command, Server& server, Client& client)
{
	std::string					join_params;
	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;

	join_params = eraseCommandfromLine(line, command);
	(void)server;
	(void)client;
	if (join_params.empty() == true)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, command.c_str(), NULL, client, channels[0].c_str());
		return ;
	}
	checkJoinParams(join_params, &channels, &passwrds);
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (server.getChannelMap().find(channels[i]) == server.getChannelMap().end())
		{
			server.setChannelMap(channels[i], client.getClientSocket());
		}
		// server.getChannelMap()[channels[i]].newClient(passwrds[i], client);
		// addClientToChannel(channels[i], passwrds[i], client, server);
	}

	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un channel et le rejoindre
}



// PASS

void		Commands::commandPASS(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientPassword() != EMPTY)
	{
		Utils::sendErrorMessage(ERR_ALREADYREGISTERED, NULL, NULL, client, NULL);
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);

	if (commandParameterExists(password, command, client) == false)
		return ;

	if (isValidPassword(password, client, server) == true)
		client.setPassword(password, server.getIrssi());
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

	if (commandParameterExists(nickname, command, client) == false)
		return ;

	if (isValidNickname(nickname, server) == true)
		client.setNickname(nickname, server.getIrssi());
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

	if (commandParameterExists(userInfo, command, client) == false)
		return ;


	const std::vector<std::string>	parameters = Utils::splitParameters(userInfo);

	if (areValidUserParameters(parameters) == false)
		return ;

	client.setUsername(parameters[0], server.getIrssi());
	Utils::displayWelcomeMessage(client);
}



// CAP

void		Commands::commandCAP(const std::string& line, const std::string& command, Client& client, Server& server)
{
	const std::string	parameter = eraseCommandfromLine(line, command);

	if (commandParameterExists(parameter, command, client) == false)
		return ;

	if (isIrssi(parameter) == true)
		server.setIrssi(true);
}
