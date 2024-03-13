#include "../inc/Commands.hpp"

bool	Commands::isValidPassword(const std::string& password, const Client& client, const Server& server)
{
	if (password != server.getPassword())
	{
		Utils::sendErrorMessage(ERR_PASSWDMISMATCH, client);
		return (false);
	}
	return (true);
}
