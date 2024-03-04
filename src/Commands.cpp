#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/defines.hpp"


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

	server.executeCommand(line, command, *this);
}



std::string		Client::getCommandFromLine(const std::string& line) const
{
	std::string	command;

	for(size_t i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) == true)
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


// Command Functions

void		Server::commandJOIN(const std::string& line, const std::string& command) const
{
	std::string		str;
	str = eraseCommandfromLine(line, command);
	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un channel et le rejoindre
}



void		Server::commandNICK(const std::string& line, const std::string& command, Client& client) const
{
	std::string		nickname;
	nickname = eraseCommandfromLine(line, command);

	try
	{
		if (isValidNickname(nickname) == true)
			client.setNickname(nickname);
	}
	catch (Server::Exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}



std::string		Server::eraseCommandfromLine(const std::string& line, const std::string& command) const
{
	std::string		res;
	res = line.substr(command.size() + 1);
	return (res);
}



void		Server::executeCommand(const std::string& line, const std::string& command, Client& client) const
{
	if (command == "JOIN")
		commandJOIN(line, command);
	else if (command == "NICK")
		commandNICK(line, command, client);
	else
		return ;
}
