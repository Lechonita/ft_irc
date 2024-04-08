#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "../inc/Utils.hpp"
# include "../inc/Client.hpp"
# include "../inc/Server.hpp"
# include "../inc/Channel.hpp"
# include "../inc/defines.hpp"

class Client;

class Server;

class Commands
{

	public:

		Commands();
		~Commands() {}

		// Command functions
		void				commandJOIN(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandKICK(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandPART(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandMODE(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandPRIVMSG(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandPASS(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandNICK(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandUSER(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandCAP(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandQUIT(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandINVITE(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandPING(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandTOPIC(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandWHOIS(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandWHO(const std::string& line, const std::string& command, Client& client, Server& server);
		void				commandDebugCLIENTS(const std::string& line, const std::string& command, Client& client, Server& server);

		// Command Utils
		static std::string	getCommandFromLine(const std::string& line);
		static std::string	eraseCommandfromLine(const std::string& line, const std::string& command);
		static void			findCommandInMessage(std::string& line, Server& server, Client& client);
		static bool			isParameterSetUp(const std::string& parameter, const Client& client, const std::string& errorMessage);
		void				executeCommand(std::string& line, std::string& command, Server& server, Client& client);

		// join_utils
		static void			checkJoinParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds);

		// kick_utils
		static void			checkKickParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds, std::string *message);

		// mode_utils
		static void			checkModeParams(std::string mode_params, std::vector<std::string> *channels, std::vector<std::string> *modes_with_args, std::vector<std::string> *modes_without_args, std::vector<std::string> *modes_args, Client& client);

		// privmsg_utils
		static void			checkPrivmsgParams(std::string parameters, std::vector<std::string> *receivers, std::string *message);

		// part_utils
		static void			checkPartParams(std::string part_params, std::vector<std::string> *channels, std::string *message);

		// nick_utils
		static bool			isValidNickname(const std::string& nickname, Client& client, const Server& server);
		static bool			nicknameAlreadyExists(const std::string& nickname, const Client& client, const Server& server);

		// pass_utils
		static bool			isValidPassword(const std::string& password, const Client& client, const Server& server);

		// user_utils
		static bool			areValidUserParameters(const std::vector<std::string> parameters);
		static void			addUserInformation(std::vector<std::string> parameters, Client& client);

		// cap_utils
		static bool			isIrssi(const std::string& line);

		// invite_utils
		static bool			areValidInviteParameters(const std::vector<std::string> parameters, const Client& client, const Server& server);

		// topic_utils
		static bool			areValidTopicParameters(const std::vector<std::string> parameters, const Client& client, const Server& server);
		static void			chooseAndExecuteTopicAction( std::vector<std::string> parameters, const Client& client);

		// whois_utils
		static size_t		getParameterType(const std::string& parameter, const Server& server);


	private:
		typedef void (Commands::*functionPointer)(const std::string&, const std::string&, Client&, Server&);
		std::map<std::string, functionPointer>		_cmdMap;
};

#endif
