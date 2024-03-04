#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/************************** CLIENT CLASS FUNCTIONS ******************************/
/********************************************************************************/


static bool	 isSpace(const char& c)
{
	return (std::isspace(static_cast<unsigned char>(c)));
}



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
		if (isSpace(line[i]) == true)
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


void		Server::commandJOIN(const std::string& str) const
{
	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un serveur et le rejoindre
}



void		Server::executeCommand(const std::string& line, const std::string& command) const
{
	if (command == "JOIN")
		commandJOIN(line);
	else
		return ;
}
