#include "../inc/Commands.hpp"



bool		Commands::areValidUserParameters(const std::vector<std::string> parameters)
{
	// <username> <hostname> <servername//IP address of user> <realfirstname> <reallastname>

	// alice * 192.168.1.10 :Alice Smith

	if (parameters.size() != EXPECTED_USER_PARAM)
		return (false);

	if (parameters[3][0] != ':')
		return (false);

	return (true);
}


void	Commands::addUserInformation(std::vector<std::string> parameters, Client& client)
{
	client.setUsername(parameters[0]);
	client.setRealName(parameters);
	std::cout << "Adding USER to status count" << std::endl;
	client.setClientStatus(NICK_USER_LOGIN);

	if (client.getClientStatus() == CONNECTED)
	{
		Utils::displayWelcomeMessage(client);
		client.setClientStatus(DISPLAY_WELCOME);
	}
}
