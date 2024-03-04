#include "../inc/Server.hpp"


// Command NICK


static bool	nicknameAlreadyExists(const std::string& nickname, const Server& server)
{
	std::map<int, Client>::const_iterator	it;
	for (it = server.getClientMap().begin(); it != server.getClientMap().end(); ++it)
	{
		if (it->second.getClientNickname() == nickname)
			return (true);
	}
	return (false);
}



static bool	isValidCharacter(const char c)
{
	return (isalnum(c) || c == '_'  || c == '-');
}



bool	Server::isValidNickname(const std::string& nickname) const
{
	for(size_t i = 0; i < nickname.size(); ++i)
	{
		if (isspace(nickname[i]) == true ||
			isValidCharacter(nickname[i]) == false)
			return (false);
	}

	if (nickname.size() > 9)
		return (false);

	if (nicknameAlreadyExists(nickname, *this) == true)
		return (false);

	return (true);
}
