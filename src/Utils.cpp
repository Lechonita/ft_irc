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




std::string Utils::getFormattedMessage(const std::string &message, const Client &client, const std::string channelName)
{
	const std::string pattern[PATTERN_COUNT_CHAN][2] = {
		{"<command>", client.getLastCommand()},
		{"<arg>", client.getLastArgument()},
		{"<client>", client.getClientNickname()},
		{"<servername>", SERVER_NAME},
		{"<oldnickname>", client.getClientOldNickname()},
		{"<nickname>", client.getClientNickname()},
		{"<clientIP", client.getClientIP()},
		{"<username>", client.getClientUsername()},
		{"<channelName>", channelName},
		{"<nicknames>", getClientListInChannel(client, channelName)},
		{"<topic>", getChannelTopic(channelName, client)}};

	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT_CHAN; ++i)
	{
		formattedMessage = replacePattern(formattedMessage, pattern[i][0], pattern[i][1]);
	}
	return (formattedMessage + END_MSG);
}

std::string		Utils::getFormattedMessage(const std::string &message, const Client &client)
{
	const std::string pattern[PATTERN_COUNT][2] = {
		{"<command>", client.getLastCommand()},
		{"<arg>", client.getLastArgument()},
		{"<client>", client.getClientNickname()},
		{"<servername>", SERVER_NAME},
		{"<oldnickname>", client.getClientOldNickname()},
		{"<nickname>", client.getClientNickname()},
		{"<clientIP", client.getClientIP()},
		{"<username>", client.getClientUsername()}};

	std::string formattedMessage = message;

	for (size_t i = 0; i < PATTERN_COUNT; ++i)
	{
		formattedMessage = replacePattern(formattedMessage, pattern[i][0], pattern[i][1]);
	}
	return (formattedMessage + END_MSG);
}



void Utils::sendFormattedMessage(const std::string &message, const Client &client, const std::string channelName)
{
	std::string formattedMessage = Utils::getFormattedMessage(message, client, channelName);

	if (send(client.getClientSocket(), formattedMessage.c_str(), formattedMessage.length(), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
	else
		std::cout << OUTGOING_MSG << formattedMessage;
}


void	Utils::sendFormattedMessage(const std::string &message, const Client &client)
{
	std::string formattedMessage = Utils::getFormattedMessage(message, client);

	if (send(client.getClientSocket(), formattedMessage.c_str(), formattedMessage.length(), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
	else
		std::cout << OUTGOING_MSG << formattedMessage;
}




// Send messages

void	Utils::sendMessage(const std::string &message, const Client &client)
{
	if (send(client.getClientSocket(), message.c_str(), message.length(), 0) == ERROR)
	{
		perror(PERR_SEND);
	}
	else
	{
		std::cout << OUTGOING_MSG << message;
	}
}




// [ Welcome Message ]


static std::string	WelcomeLine1(const Client &client)
{
	return (":" + std::string(SERVER_NAME) + " 001 " + client.getClientNickname() + " :Welcome to the Internet Relay Network " + client.getClientNickname() + "!" + client.getClientUsername() + "@" + client.getClientIP() + '\n');
}



static std::string	WelcomeLine2(const Client &client)
{
	return (":" + std::string(SERVER_NAME) + " 002 " + client.getClientNickname() + " :Your host is irc, running version 0.6\n");
}



static std::string	WelcomeLine3(const Client &client)
{
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	std::stringstream ss;
	ss << ":" + std::string(SERVER_NAME) + " 003 " << client.getClientNickname() << " :This server was created on ";
	ss << (localTime->tm_year + 1900) << "/";
	ss << (localTime->tm_mon + 1) << "/";
	ss << localTime->tm_mday << '\n';

	return ss.str();
}


static std::string	WelcomeLine4(const Client &client)
{
	return ("004 " + client.getClientNickname() + " :" + std::string(SERVER_NAME) + " 1.0 -none- itkol\n");
}


void	Utils::displayWelcomeMessage(const Client& client)
{
	if (send(client.getClientSocket(), WelcomeLine1(client).c_str(), WelcomeLine1(client).length(), 0) == ERROR ||
		send(client.getClientSocket(), WelcomeLine2(client).c_str(), WelcomeLine2(client).length(), 0) == ERROR ||
		send(client.getClientSocket(), WelcomeLine3(client).c_str(), WelcomeLine3(client).length(), 0) == ERROR ||
		send(client.getClientSocket(), WelcomeLine4(client).c_str(), WelcomeLine4(client).length(), 0) == ERROR)
	{
		std::perror(PERR_SEND);
	}
	else
	{
		std::cout << OUTGOING_MSG << WelcomeLine1(client);
		std::cout << OUTGOING_MSG << WelcomeLine2(client);
		std::cout << OUTGOING_MSG << WelcomeLine3(client);
		std::cout << OUTGOING_MSG << WelcomeLine4(client);
	}
}



void	Utils::joinMessageSuccessful(const Client& client, Server& server, std::string channel_name)
{
	std::string						message1 = ":" + client.getClientNickname()
												+ "!~" + client.getClientUsername() + "@"
												+ client.getClientIP() + " JOIN :" + channel_name;
	std::string						message2 = ":ircserv " + getFormattedMessage(RPL_NAMREPLY, client, channel_name);
	std::string						message3 = ":ircserv " + getFormattedMessage(RPL_ENDOFNAMES, client);

	message2.erase(message2.size() - 2, 2);
	message3.erase(message3.size() - 2, 2);

	server.sendMessageToChannel(channel_name, message1, client);
	sendFormattedMessage(message2, client, channel_name);
	sendFormattedMessage(message3, client, channel_name);
}



void	Utils::partMessage(const Client& client, Server& server, const std::string channel_name, const std::string message)
{
	std::string						full_message = ":" + client.getClientNickname()
												+ "!~" + client.getClientUsername() + "@"
												+ client.getClientIP() + " PART " + channel_name;

	if (message != "")
		full_message = full_message + " :" + message;
	server.sendMessageToChannel(channel_name, full_message, client);
}



void	Utils::kickMessageSuccessfull(const Client& client, Server& server, const std::string channel_name, const std::string message, std::string client_kicked)
{
	std::string						full_message = ":" + client.getClientNickname()
												+ "!~" + client.getClientUsername() + "@"
												+ client.getClientIP() + " KICK " + channel_name;

	if (message == "")
		full_message = full_message + " "  + client_kicked + " :" + client.getClientNickname();
	else
		full_message = full_message + " " + client_kicked + " :" + message;
	server.sendMessageToChannel(channel_name, full_message, client);
}

// Util functions

std::vector<std::string>		Utils::splitParameters(const std::string& userInfo)
{
	std::vector<std::string>	parameters;

	char *token = strtok((char *)userInfo.c_str(), SPACE);
	while (token != NULL && userInfo.empty() == false)
	{
		parameters.push_back(token);
		token = strtok(NULL, SPACE);
	}
	return(parameters);
}




std::string		Utils::getClientListInChannel(const Client &client, const std::string channelName)
{
	std::vector<Channel*>	channels = client.getClientChannels();
	std::string				client_list = "";
	size_t					pos;

	if (channels.empty() == true)
		return ("");
	for (pos = 0; pos < channels.size(); pos++)
	{
		if (channels[pos]->getChannelName() == channelName)
			break ;
	}
	if (pos == channels.size())
		return ("");
	std::vector<channelClient>	clients = channels[pos]->getChannelClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].isOperator == true)
			client_list += "@";
		client_list += clients[i].client->getClientNickname();
		if (i != clients.size() - 1)
			client_list += " ";
	}
	return (client_list);
}



std::string		Utils::getClientListInServer(const Server& server)
{
	std::map<int, Client>	clients = server.getClientMap();
	std::string						clientList = "";

	std::map<int, Client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		clientList += it->second.getClientNickname();
		clientList += " ";
	}
	return (clientList);
}


std::string		Utils::getChannelListInServer(const Server& server)
{
	std::map<std::string, Channel>	channels = server.getChannelMap();
	std::string						channelList = "";

	std::map<std::string, Channel>::iterator	it;
	for (it = channels.begin(); it != channels.end(); ++it)
	{
		channelList += it->second.getChannelName();
		channelList += " ";
	}
	return (channelList);
}



std::vector<std::string>		Utils::getChannelListInClient(const Client& client)
{
	std::vector<Channel*>		channels = client.getClientChannels();
	std::vector<std::string>	channelList;

	std::vector<Channel*>::iterator	it;
	for (it = channels.begin(); it != channels.end(); ++it)
	{
		channelList.push_back((*it)->getChannelName());
	}
	return (channelList);
}



bool		Utils::channelExists(const Server& server, const std::string& channelname)
{
	const std::string	channelList = Utils::getChannelListInServer(server);

	if (channelList.find(channelname) != std::string::npos)
		return (true);
	return (false);
}



std::string		Utils::getChannelTopic(const std::string& channelname, const Client& client)
{
	std::vector<Channel*>					channels = client.getClientChannels();
	std::vector<Channel*>::const_iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannelName() == channelname)
		{
			return ((*it)->getChannelTopic());
		}
	}
	return (EMPTY);
}
