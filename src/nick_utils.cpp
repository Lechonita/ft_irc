#include "../inc/Commands.hpp"


// Command NICK


bool	Commands::nicknameAlreadyExists(const std::string& nickname, const Client& client, const Server& server)
{
	std::map<int, Client> clients = server.getClientMap();
	if (server.getClientMap().empty() == false)
	{
		std::map<int, Client>::const_iterator	it;
		for (it = clients.begin(); it != clients.end(); ++it)
		{
			std::string	clientNickname = it->second.getClientNickname();
			bool		nickOK = it->second.getNicknameOKFlag();
			int			socket = it->second.getClientSocket();

			if (clientNickname != EMPTY && clientNickname == nickname && nickOK == true && client.getClientSocket() != socket)
				return (true);
		}
	}
	return (false);
}



static bool	isValidCharacter(const char c)
{
	return (isalnum(c) || c != '@'  || c != '+' || c != '%' || c != '&');
}



bool	Commands::isValidNickname(const std::string& nickname, Client& client, const Server& server)
{
	for(size_t i = 0; i < nickname.size(); ++i)
	{
		if (isspace(nickname[i]) != NOT_WHITESPACE ||
			isValidCharacter(nickname[i]) == false ||
			nickname.size() > MAX_NICK_LEN)
		{
			Utils::sendFormattedMessage(ERR_ERRONEUSNICKNAME, client);
			return (false);
		}
	}

	if (nicknameAlreadyExists(nickname, client, server) == true)
	{
		client.setOldNickname(nickname);
		Utils::sendFormattedMessage(ERR_NICKNAMEINUSE, client);
		return (false);
	}
	return (true);
}
