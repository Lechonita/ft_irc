#include "../inc/Commands.hpp"


void	Commands::findCommandInMessage(std::string& line, Server& server, Client& client)
{
	std::string		command = getCommandFromLine(line);
	Commands		cmd;

	if (command.empty() == true)
		return ;

	client.setLastCommand(command);

	cmd.executeCommand(line, command, server, client);
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



void		Commands::executeCommand(std::string& line, std::string& command, Server& server, Client& client)
{
	std::map<std::string, functionPointer>::iterator it;
	bool			correctCmd = false;

	for (it = _cmdMap.begin(); it != _cmdMap.end(); ++it)
	{
		if (it->first == command)
		{
			(this->*(it->second))(line, command, client, server);
			correctCmd = true;
		}
	}
	if (correctCmd == false)
	{
		Utils::sendFormattedMessage(ERR_UNKNOWNCOMMAND, client);
	}
}



std::string		Commands::eraseCommandfromLine(const std::string& line, const std::string& command)
{
	if (line.length() == command.length())
		return (EMPTY);
	return (line.substr(command.size() + 1));
}



bool	Commands::isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage)
{
	if (parameter == EMPTY)
	{
		if (errorMessage != EMPTY)
			Utils::sendFormattedMessage(errorMessage, client);
		return (false);
	}
	return (true);
}


// bool	Commands::isNicknameSetUp(const Client& client)
// {
// 	if (client.getNicknameOKFlag() == false)
// 		return (false);
// 	return (true);
// }