#ifndef UTILS_HPP
# define UTILS_HPP

#include "../inc/defines.hpp"
#include "../inc/Client.hpp"

class Client;

class Utils
{

	public:

	// Error management
	static void			sendErrorMessage(
			const std::string& message, const std::string& command, const std::string& arg, const Client& client, const std::string& channelName);

	static std::string	getFormattedMessage(
			const std::string& message, const std::string& command, const std::string& arg, const Client& client, const std::string& channelName);

	static std::string	replacePattern(
			std::string& message, const std::string& toChange, const std::string& replacement);


	// Send messages
	static void			sendMessage(const std::string& message, const Client& client);

};

#endif