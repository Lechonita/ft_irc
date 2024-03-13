#include "../inc/Commands.hpp"


// JOIN


void		Commands::commandJOIN(const std::string& line, const std::string& command, Server& server, Client& client)
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

	if (isParameterSetUp(nickname, client, EMPTY) == false)
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

	if (isParameterSetUp(userInfo, client, EMPTY) == false)
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

	if (isParameterSetUp(parameter, client, EMPTY) == false)
		return ;

	if (isIrssi(parameter) == true)
		server.setIrssi(true);
}
