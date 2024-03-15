#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "../inc/Utils.hpp"
# include "../inc/Client.hpp"
# include "../inc/Server.hpp"
#include "../inc/defines.hpp"

class Client;

class Server;

class Commands
{

	public:

		Commands();
		~Commands() {}

		// Command functions
		static void			commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandPASS(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandNICK(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandUSER(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandCAP(const std::string& line, const std::string& command, Client& client, Server& server);
		static void			commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server);

		// Command Utils
		static std::string	getCommandFromLine(const std::string& line);
		static std::string	eraseCommandfromLine(const std::string& line, const std::string& command);
		static void			findCommandInMessage(const std::string& line, Server& server, Client& client);
		static bool			isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage);
		void				executeCommand(std::string& line, std::string& command, Server& server, Client& client);

		// join_utils
		static void			checkJoinParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds);
		
		// nick_utils
		static bool			isValidNickname(const std::string& nickname, const Server& server);
		
		// pass_utils
		static bool			isValidPassword(const std::string& password, const Client& client, const Server& server);
		
		// user_utils
		static bool			areValidUserParameters(const std::vector<std::string> parameters);
		
		// cap_utils
		static bool			isIrssi(const std::string& line);



	private:
		typedef void (Commands::*functionPointer)(const std::string&, const std::string&, Client&, Server&);
		std::map<std::string, functionPointer>		_cmdList;
};

#endif
