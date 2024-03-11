#include "../inc/Commands.hpp"

bool	Commands::isValidPassword(const std::string& password, const Client& client, const Server& server)
{
	if (password != server.getPassword())
		Utils::sendErrorMessage(ERR_PASSWDMISMATCH, "", "", client, "");
	// {
	// 	// 464 ERR_PASSWDMISMATCH
	// 	if (send(client.getClientSocket(), ERR_PASSWDMISMATCH,  strlen(ERR_PASSWDMISMATCH), 0) == ERROR)
	// 	{
	// 		perror(PERR_SEND);
	// 	}
	// 	return (false);
	// }

	return (true);
}
