#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "../inc/Client.hpp"
# include "../inc/Server.hpp"

class Client;

class Server;

class Commands
{

	public:

		// Command functions
		static void			commandJOIN(const std::string& line, const std::string& command);
		static void			commandPASS(const std::string& line, const std::string& command, Client& client);
		static void			commandNICK(const std::string& line, const std::string& command, Client& client, const Server& server);
		
		
		// Util functions
		static void			findCommandInMessage(const std::string& line, const Server& server, Client& client);
		static std::string	getCommandFromLine(const std::string& line);
		static void			executeCommand(const std::string& line, const std::string& command, const Server& server, Client& client);
		static std::string	eraseCommandfromLine(const std::string& line, const std::string& command);
		static bool			isValidNickname(const std::string& nickname, const Server& server);




	private:

};

#endif


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
