#include "../inc/Commands.hpp"


void	Server::sendUserInformation(const std::vector<std::string> parameters, const Client& client)
{
	// This information typically includes the user's nickname, username, hostname, real name, and sometimes additional details such as their channels, server operator status, and idle time.
	
	std::string line[USER_INFO_LINES];
	std::map<int, Client>::iterator	it;

	for (it = _clientMap.begin(); it != _clientMap.end(); ++it)
	{
		if (it->second.getClientNickname() == parameters[0])
		{
			std::string	status;
			if (it->second.getClientStatus() == CONNECTED)
				status = "Connected";
			else
				status = "Disconnected";

			line[0] = "Nickname :" + it->second.getClientNickname() + NEWLINE;
			line[1] = "Username :" + it->second.getClientUsername() + NEWLINE;
			line[2] = "Hostname :" + std::string(HOST_NAME) + NEWLINE;
			line[3] = "Real name :" + it->second.getClientRealName() + NEWLINE;
			line[4] = "Connextion status :" + status + NEWLINE;
		}
	}

	for(size_t i = 0; i < USER_INFO_LINES; ++i)
	{
		if (send(client.getClientSocket(), line[i].c_str(), line[i].length(), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		else
		{
			std::cout << OUTGOING_MSG << line[i];
		}
	}
}

void	Server::sendChannelInformation(const std::vector<std::string> parameters, const Client& client)
{
	// information about the channel, including its name, topic, and a list of users who are currently in the channel.

	std::string line[CHAN_INFO_LINES];
	std::map<std::string, Channel>::iterator	it;

	for (it = _channelMap.begin(); it != _channelMap.end(); ++it)
	{
		if (it->second.getChannelName() == parameters[0])
		{
			line[0] = "Name :" + it->second.getChannelName() + NEWLINE;
			line[1] = "Topic :" + it->second.getChannelTopic() + NEWLINE;
			line[2] = "Client List :" + Utils::getClientListInChannel(client, parameters[0]) + NEWLINE;
		}
	}

	for(size_t i = 0; i < CHAN_INFO_LINES; ++i)
	{
		if (send(client.getClientSocket(), line[i].c_str(), line[i].length(), 0) == ERROR)
		{
			perror(PERR_SEND);
		}
		else
		{
			std::cout << OUTGOING_MSG << line[i];
		}
	}
}



size_t	Commands::getParameterType(const std::string& parameter, const Server& server)
{
	const std::string	clients = Utils::getClientListInServer(server);
	const std::string	channels = Utils::getChannelListInServer(server);

	if (clients.find(parameter) != std::string::npos)
		return (USER_TYPE);
	if (channels.find(parameter) != std::string::npos)
		return (CHANNEL_TYPE);
	return (UNKNOWN_TYPE);
}
