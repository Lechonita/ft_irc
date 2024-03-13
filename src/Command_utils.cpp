#include "../inc/Commands.hpp"


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
	else if (command == "CAP")
		Commands::commandCAP(line, command, client, server);
	else if (command == "PASS")
		Commands::commandPASS(line, command, client, server);
	else if (command == "NICK")
		Commands::commandNICK(line, command, client, server);
	else if (command == "USER")
		Commands::commandUSER(line, command, client, server);
	else
	{
		Utils::sendErrorMessage(ERR_UNKNOWNCOMMAND, client);
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
		if (errorMessage != EMPTY)
			Utils::sendErrorMessage(errorMessage, client);
		return (false);
	}
	return (true);
}
