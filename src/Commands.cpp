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
	printf("Je rentre dans findCommandInMessage pour chopper la commande\n");
	const std::string		command = getCommandFromLine(line);

	printf("  J'obtiens la commande -%s-\n", command.c_str());
	if (command.empty() == true)
		return ;

	// if (isCommandFromList(command, server) == false)
	// 	return ;

	server.executeCommand(line, command);
}



std::string		Client::getCommandFromLine(const std::string& line) const
{
	std::string	command;
	
	printf("  Dans getCommandFromLine, je recois la ligne -%s-\n", line.c_str());

	for(size_t i = 0; i < line.size(); ++i)
	{
		printf("   Je suis en train de regarder -%c-\n", line[i]);
		if (isSpace(line[i]) == true)
			break ;
		command += line[i];
	}
	return (command);
}


// bool	Client::isCommandFromList(const std::string& command, const Server& server) const
// {
// 	printf(" Je rentre dans isCommandFromList\n");
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
	printf("Dans JOIN, j'imprime la commande : %s\n", str.c_str());
	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un serveur et le rejoindre
}



void		Server::executeCommand(const std::string& line, const std::string& command) const
{
	printf("Je rentre dans executeCommand\n");
	if (command == "JOIN")
		commandJOIN(line);
	else
		return ;
}
