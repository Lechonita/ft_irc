#include "../inc/Commands.hpp"
#include "../inc/defines.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

/********************************************************************************/
/****************************** UTIL FUNCTIONS **********************************/
/********************************************************************************/


void	Commands::findCommandInMessage(const std::string& line, Server& server, Client& client)
{
	const std::string		command = getCommandFromLine(line);

	if (command.empty() == true)
		return ;

	// if (isCommandFromList(command, server) == false)
	// 	return ;

	Commands::executeCommand(line, command, server, client);
}



std::string		Commands::getCommandFromLine(const std::string& line)
{
	std::string	command;

	for(size_t i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) != NOT_WHITESPACE)
			break ;
		command += line[i];
	}
	return (command);
}



void		Commands::executeCommand(const std::string& line, const std::string& command, Server& server, Client& client)
{
	if (command == "JOIN")
		Commands::commandJOIN(line, command, server, client);
	else if (command == "PASS")
		Commands::commandPASS(line, command, client, server);
	else if (command == "NICK")
		Commands::commandNICK(line, command, client, server);
	else if (command == "USER")
		Commands::commandUSER(line, command, client, server);
	else
	{
		Utils::sendErrorMessage(ERR_UNKNOWNCOMMAND, command, "", client, "");
	}
	std::map<std::string, Channel>	test = server.getChannelMap();
	std::map<std::string, Channel>::iterator	it;

	for (it = test.begin(); it != test.end(); it++)
	{
		std::cout << GREEN << "Channel name: " << it->first << NC << std::endl;
	}
}



std::string		Commands::eraseCommandfromLine(const std::string& line, const std::string& command)
{
	if (line.length() == command.length())
		return (EMPTY);

	std::string		res;
	res = line.substr(command.size() + 1);
	return (res);
}



bool	Commands::isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage)
{
	if (parameter == EMPTY)
	{
		Utils::sendErrorMessage(errorMessage, "", "", client, "");
		return (false);
	}
	return (true);
}



bool	Commands::commandParameterExists(const std::string& parameter, const std::string& command, const Client& client)
{
	if (parameter == EMPTY)
	{
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, command, "", client, "");
		return (false);
	}
	return (true);
}


/********************************************************************************/
/***************************** COMMAND FUNCTIONS ********************************/
/********************************************************************************/


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
		Utils::sendErrorMessage(ERR_NEEDMOREPARAMS, command, NULL, client, channels[0]);
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
		Utils::sendErrorMessage(ERR_ALREADYREGISTERED, "", "", client, "");
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);

	if (commandParameterExists(password, command, client) == false)
		return ;

	if (isValidPassword(password, client, server) == true)
		client.setPassword(password);
}



// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
		return ;

	const std::string	nickname = eraseCommandfromLine(line, command);

	if (commandParameterExists(nickname, command, client) == false)
		return ;

	if (isValidNickname(nickname, server) == true)
		client.setNickname(nickname);
}



// USER

void		Commands::commandUSER(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (isParameterSetUp(client.getClientPassword(), client, PASS_NOT_ENTERED) == false)
		return ;

	if (isParameterSetUp(client.getClientNickname(), client, NICK_NOT_ENTERED) == false)
		return ;

	const std::string	userInfo = eraseCommandfromLine(line, command);

	if (commandParameterExists(userInfo, command, client) == false)
		return ;

	if (areValidUserParameters(userInfo, server) == true)
		return ;
}
