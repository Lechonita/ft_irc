#include "../inc/Utils.hpp"

// Error management

std::string Utils::replacePattern(std::string &message, const std::string &from, const std::string &to)
{
	size_t startPos = 0;

	while ((startPos = message.find(from, startPos)) != std::string::npos)
	{
		message.replace(startPos, from.length(), to);
		startPos += to.length();
	}
	return (message);
}

// Command, Arg, Client, ChannelName
std::string Utils::getFormattedMessage(const std::string &message, const std::string &command, const std::string &arg, const Client &client, const std::string &channelName)
{
	const std::string pattern[PATTERN_COUNT][2] = {
		{"<command>", command}, {"<arg>", arg},
		{"<client>", client.getClientNickname()},
		{"ChannelName", channelName}};

	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT; ++i)
	{
		formattedMessage = Utils::replacePattern(formattedMessage, pattern[i][0], pattern[i][1]);
	}
	return (formattedMessage);
}

void Utils::sendErrorMessage(const std::string &message, const std::string &command, const std::string &arg, const Client &client, const std::string &channelName)
{
	std::string formattedMessage = Utils::getFormattedMessage(message, command, arg, client, channelName);

	if (send(client.getClientSocket(), formattedMessage.c_str(), formattedMessage.length(), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
	else
		std::cout << OUTGOING_MSG << formattedMessage;
}

// Send messages

void Utils::sendMessage(const std::string &message, const Client &client)
{
	if (send(client.getClientSocket(), message.c_str(), message.length(), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
	else
		std::cout << OUTGOING_MSG << message;
}
