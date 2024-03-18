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

		Commands();
		~Commands() {}

		// Command functions
		void			commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandPART(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandPRIVMSG(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandPASS(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandNICK(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandUSER(const std::string& line, const std::string& command, Client& client, Server& server);
		void			commandCAP(const std::string& line, const std::string& command, Client& client, Server& server);\
		void			commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server);

		// Command Utils
		static std::string	getCommandFromLine(const std::string& line);
		static std::string	eraseCommandfromLine(const std::string& line, const std::string& command);
		static void			findCommandInMessage(std::string& line, Server& server, Client& client);
		static bool			isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage);
		void				executeCommand(std::string& line, std::string& command, Server& server, Client& client);

		// join_utils
		static void			checkJoinParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds);
		static void			checkPrivmsgParams(std::string parameters, std::vector<std::string> *receivers, std::string *message);
		static void			checkPartParams(std::string part_params, std::vector<std::string> *channels, std::string *message);

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
		std::map<std::string, functionPointer>		_cmdMap;
};

#endif
