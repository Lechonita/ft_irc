#ifndef UTILS_HPP
# define UTILS_HPP

#include "../inc/defines.hpp"
#include "../inc/Client.hpp"

class Client;

class Utils
{

	public:

	// Error management
	static void							sendErrorMessage(
											const std::string& message, const char* command, const char* arg, const Client client, const char* channelName);

	static std::string					getFormattedMessage(
											const std::string& message, const char* command, const char* arg, const Client client, const char* channelName);

	static std::string					replacePattern(
											std::string& message, const std::string& toChange, const std::string& replacement);


	// Send messages
	static void							sendMessage(const std::string& message, const Client& client);
	static void							displayWelcomeMessage(const Client& client);

	// utils functions
	static std::vector<std::string>		splitParameters(const std::string& userInfo);

};

#endif