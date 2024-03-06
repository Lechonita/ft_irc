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
	else
	{
		if (send(client.getClientSocket(), ERR_UNKNOWNCOMMAND,  strlen(ERR_UNKNOWNCOMMAND), 0) == ERROR)
		{
			// 421 ERR_UNKNOWNCOMMAND <command>
			perror(PERR_SEND);
		}
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
		if (send(client.getClientSocket(), PASS_ALREADY_ENTERED,  strlen(PASS_ALREADY_ENTERED), 0) == ERROR)
		{
			// 462 ERR_ALREADYREGISTRED
			perror(PERR_SEND);
		}
		return ;
	}

	const std::string	password = eraseCommandfromLine(line, command);
	if (password == EMPTY)
	{
		// 461 ERR_NEEDMOREPARAMS <command>
		if (send(client.getClientSocket(), ERR_NEEDMOREPARAMS,  strlen(ERR_NEEDMOREPARAMS), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		return ;
	}

	if (isValidPassword(password, client, server) == true)
		client.setPassword(password);
}



// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, const Server& server)
{
	if (client.getClientPassword() == EMPTY)
	{
		if (send(client.getClientSocket(), PASS_NOT_ENTERED,  strlen(PASS_NOT_ENTERED), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		return ;
	}

	const std::string	nickname = eraseCommandfromLine(line, command);
	if (nickname == EMPTY)
	{
		if (send(client.getClientSocket(), ERR_NEEDMOREPARAMS,  strlen(ERR_NEEDMOREPARAMS), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		return ;
	}

	if (isValidNickname(nickname, server) == true)
		client.setNickname(nickname);
}
