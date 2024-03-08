#include "../inc/Commands.hpp"
#include "../inc/defines.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


/********************************************************************************/
/****************************** UTIL FUNCTIONS **********************************/
/********************************************************************************/


void	Commands::findCommandInMessage(const std::string& line, const Server& server, Client& client)
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



void		Commands::executeCommand(const std::string& line, const std::string& command, const Server& server, Client& client)
{
	if (command == "JOIN")
		Commands::commandJOIN(line, command);
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

void		Commands::commandJOIN(const std::string& line, const std::string& command)
{
	std::string		str;
	str = eraseCommandfromLine(line, command);
	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un channel et le rejoindre
}



// PASS

void		Commands::commandPASS(const std::string& line, const std::string& command, Client& client, const Server& server)
{
	if (client.getClientPassword() != EMPTY)
	{
		Utils::sendErrorMessage(ERR_ALREADYREGISTRED, "", "", client, "");
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);

	if (commandParameterExists(password, command, client) == false)
		return ;

	if (isValidPassword(password, client, server) == true)
		client.setPassword(password);
}



// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, const Server& server)
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

void		Commands::commandUSER(const std::string& line, const std::string& command, Client& client, const Server& server)
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
