#include "../inc/Commands.hpp"


// Command NICK


bool	Commands::nicknameAlreadyExists(const std::string& nickname, const Server& server)
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



bool	Commands::isValidNickname(const std::string& nickname, const Client& client)
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
	return (true);
}



// static std::string	changeDigits(const std::string& nickname)
// {
// 	std::string newNickname = nickname;

// 	size_t pos = newNickname.size();
// 	while (pos > 0 && isdigit(newNickname[pos - 1]))
// 		pos--;

// 	if (pos == newNickname.size())
// 		newNickname += '1';
// 	else
// 		newNickname[pos]++;

// 	return (newNickname);
// }


// static bool		hasDigits(const std::string& nickname)
// {
// 	if (isdigit(nickname[0]) > 0)
// 		return (true);
// 	return (false);
// }



std::string		Commands::createNewNickname(std::string& nickname, const Server& server)
{
	std::string newNickname = nickname;

	printf(">>>>>> DANS LA FONCTION <<<<<<<\n");
	size_t pos = newNickname.size();
	printf("	pos = %lu\n", pos);

	while (pos > 0 && isdigit(newNickname[pos - 1]))
		pos--;

	printf("	pos apres while = %lu\n", pos);
	if (pos == newNickname.size())
		newNickname += '1';
	else
	{
		std::string	value = newNickname.substr(pos);
		// if (newNickname[pos] == '9' && newNickname[pos - 1])
		// {
		// 	newNickname[pos -]
		// }
		newNickname[pos]++;
	}

	std::map<int, Client> clients = server.getClientMap();
	if (server.getClientMap().empty() == false)
	{
		std::map<int, Client>::const_iterator	it;
		for (it = clients.begin(); it != clients.end(); ++it)
		{
			std::string	clientNickname = it->second.getClientNickname();
			if (clientNickname != EMPTY && clientNickname == newNickname)
			{
				newNickname = createNewNickname(newNickname, server);
				break ;
			}
		}
	}

	printf("	newNick = %s\n", newNickname.c_str());
	return (newNickname);


	// if (hasDigits(nickname) == true)
	// {
	// 	return (changeDigits(nickname));
	// }




// 	static int			counter;
// 	std::string			res = nickname + "_";
// 	std::stringstream	ss;

// 	std::map<int, Client> clients = server.getClientMap();
// 	if (server.getClientMap().empty() == false)
// 	{
// 		std::map<int, Client>::const_iterator	it;
// 		for (it = clients.begin(); it != clients.end(); ++it)
// 		{
// 			std::string	clientNickname = it->second.getClientNickname();
// 			if (clientNickname != EMPTY && clientNickname == nickname)
// 			{
// 				ss << counter + 1;
// 				res = nickname + ss.str();
// 			}
// 			if (clientNickname != EMPTY && clientNickname == res)
// 			{
// 				res.substr(0, res.size() - ss.str().size() + 1);
// 				printf("res apres substr = -%s-\n", res.c_str());
// 				ss << counter + 2;
// 				res = res + ss.str();
// 			}
// 		}
// 	}
// 	return (res);
}
