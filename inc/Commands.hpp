#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "../inc/Utils.hpp"
# include "../inc/Client.hpp"
# include "../inc/Server.hpp"
# include "../inc/defines.hpp"
# include "../inc/Channel.hpp"

class Client;

class Server;

class Commands
{

	public:

		// Command functions
		static void			commandJOIN(const std::string& line, const std::string& command, Server& server, Client& client);
		static void			commandPART(const std::string& line, const std::string& command, Server& server, Client& client);
		static void			commandPRIVMSG(const std::string& line, const std::string& command, Server& server, Client& client);
		static void			commandPASS(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandNICK(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandUSER(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandCAP(const std::string& line, const std::string& command, Client& client, Server& server);\
		static void			commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server);

		// Command Utils
		static std::string	getCommandFromLine(const std::string& line);
		static std::string	eraseCommandfromLine(const std::string& line, const std::string& command);
		static void			findCommandInMessage(const std::string& line, Server& server, Client& client);
		static void			executeCommand(const std::string& line, const std::string& command, Server& server, Client& client);
		static bool			isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage);

		// join_utils
		static void			checkJoinParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds);
		static void			checkPrivmsgParams(std::string parameters, std::vector<std::string> *receivers, std::string *message);
		static void			checkPartParams(std::string part_params, std::vector<std::string> *channels);

		// nick_utils
		static bool			isValidNickname(const std::string& nickname, const Server& server);

		// pass_utils
		static bool			isValidPassword(const std::string& password, const Client& client, const Server& server);

		// user_utils
		static bool			areValidUserParameters(const std::vector<std::string> parameters);

		// cap_utils
		static bool			isIrssi(const std::string& line);



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
