#include "../inc/Server.hpp"
#include "../inc/defines.hpp"


// Command NICK


static bool	nicknameAlreadyExists(const std::string& nickname, const Server& server)
{
	std::map<int, Client> clients = server.getClientMap();
	if (server.getClientMap().empty() == false)
	{
		std::map<int, Client>::const_iterator	it;
		for (it = clients.begin(); it != clients.end(); ++it)
		{
			std::string	clientNickname = it->second.getClientNickname();
			if (clientNickname != EMPTY && clientNickname == nickname)
				return (true);
		}
	}
	return (false);
}



static bool	isValidCharacter(const char c)
{
	return (isalnum(c) || c != '@'  || c != '+' || c != '%' || c != '&');
}



bool	Server::isValidNickname(const std::string& nickname) const
{
	// ERR 432 "<nick> :Erroneus nickname"
	for(size_t i = 0; i < nickname.size(); ++i)
	{
		if (isspace(nickname[i]) != NOT_WHITESPACE ||
			isValidCharacter(nickname[i]) == false)
		{
			std::cout << RED << "Error: Invalid character for nickname. Whitespaces, @, +, %, & not allowed." << NC << std::endl;
			return (false);
		}
	}

	if (nickname.size() > 9)
	{
		std::cout << RED << "Error: Maximum length for nickname is 9 characters." << NC << std::endl;
		return (false);
	}

	if (nicknameAlreadyExists(nickname, *this) == true)
	{
		std::cout << RED << "Error: Nickname already exists." << NC << std::endl;
		return (false);
	}
	return (true);
}
