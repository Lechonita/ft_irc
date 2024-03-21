#ifndef UTILS_HPP
# define UTILS_HPP

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"

class Client;

class Channel;

class Server;

class Utils
{
	public:

	// Error management
	static void							sendErrorMessage(const std::string& message, const Client& client, const std::string channelName);
	static void							sendErrorMessage(const std::string& message, const Client& client);
	static std::string					getFormattedMessage(const std::string& message, const Client& client, const std::string channelName);
	static std::string					getFormattedMessage(const std::string& message, const Client& client);
	static std::string					replacePattern(std::string& message, const std::string& toChange, const std::string& replacement);

	// Send messages
	static void							sendMessage(const std::string& message, const Client& client);
	static void							displayWelcomeMessage(const Client& client);
	static void							joinMessageSuccessful(const Client& client, Server& server, std::string channel_name);
	static void							partMessage(const Client& client, Server& server, const std::string channel_name, const std::string message = "");
	static void							kickMessageSuccessfull(const Client& client, Server& server, const std::string channel_name, const std::string message, std::string client_kicked);

	// Quit_utils
	static void							notifyQuitinChannels(const Client& client, const Server& server);
	static void							sendQuitMessagetoChannels(const std::string& nickname, const Channel channel);
	static void							displayQuitChannelMessage(const std::string& nickname, const int receiverSocket);

	// utils functions
	static std::vector<std::string>		splitParameters(const std::string& userInfo);
	static std::string					getClientListInChannel(const Client &client, const std::string channelName);
};

#endif