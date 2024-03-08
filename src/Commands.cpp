#include "../inc/Commands.hpp"
#include "../inc/defines.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"

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
		Commands::commandPASS(line, command, client);
	else if (command == "NICK")
		Commands::commandNICK(line, command, client, server);
	else
		return ;
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


void		Commands::commandJOIN(const std::string& line, const std::string& command, Server& server, Client& client)
{
	std::string					join_params;
	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;

	join_params = eraseCommandfromLine(line, command);
	(void)server;
	(void)client;
	if (join_params.empty() == true)
	{
		std::cout << Utils::sendErrorMsg(ERR_NEEDMOREPARAMS) << std::endl;
		return ;
	}
	checkJoinParams(join_params, &channels, &passwrds);
	for (int i = 0; i < channels.size(); i++)
	{
		if (server.getChannelMap().find(channels[i]) == server.getChannelMap().end())
		{
			server.setChannelMap(channels[i], client.getClientSocket());
		}
		server.getChannelMap()[channels[i]].newClient(passwrds[i], client);
		// addClientToChannel(channels[i], passwrds[i], client, server);
	}

	// si channel existe, rejoindre  le channel
	// si channel n'existe pas, créer un channel et le rejoindre
}


// PASS




// PASS

void			Commands::commandPASS(const std::string& line, const std::string& command, Client& client)
{
	(void)line;
	(void)command;
	(void)client;
}


// NICK

void		Commands::commandNICK(const std::string& line, const std::string& command, Client& client, Server& server)
{
	if (client.getClientPassword() == EMPTY)
	{
		if (send(client.getClientSocket(), NO_PASS,  strlen(NO_PASS), 0) == ERROR)
			perror(PERR_SEND);
	}

	std::string		nickname;
	nickname = eraseCommandfromLine(line, command);

	if (isValidNickname(nickname, server) == true)
		client.setNickname(nickname);
}
