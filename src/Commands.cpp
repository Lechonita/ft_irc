#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/defines.hpp"
#include "../inc/Utils.hpp"

/********************************************************************************/
/************************** CLIENT CLASS FUNCTIONS ******************************/
/********************************************************************************/



void	Client::findCommandInMessage(const std::string& line, const Server& server)
{
	const std::string		command = getCommandFromLine(line);

	if (command.empty() == true)
		return ;

	// if (isCommandFromList(command, server) == false)
	// 	return ;

	server.executeCommand(line, command);
}



std::string		Client::getCommandFromLine(const std::string& line) const
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


// bool	Client::isCommandFromList(const std::string& command, const Server& server) const
// {
// 	std::vector<std::string>::iterator	it;
// 	for(it = server.getCommandList().begin(); it != server.getCommandList().end(); ++it)
// 	{
// 		if ((*it) == command)
// 			return (true);
// 	}
// 	return (false);
// }


/********************************************************************************/
/************************** SERVER CLASS FUNCTIONS ******************************/
/********************************************************************************/


void		Server::commandJOIN(const std::string& line, const std::string& command) const
{
	std::string		channel_params;

	channel_params = eraseCommandfromLine(line, command);

	if (channel_params.empty() == true)
	{
		Utils::sendErrorMsg(ERR_NEEDMOREPARAMS);
		return ;
	}


	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un serveur et le rejoindre
}



std::string		Server::eraseCommandfromLine(const std::string& line, const std::string& command) const
{
	std::string		res;
	res = line.substr(command.size() + 1);
	return (res);
}



void		Server::executeCommand(const std::string& line, const std::string& command) const
{
	if (command == "JOIN")
		commandJOIN(line, command);
	else
	{
		std::cout << Utils::sendErrorMsg(ERR_UNKNOWNCOMMAND);
		return ;
	}
}
