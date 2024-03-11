#include "../inc/Commands.hpp"





bool		Commands::areValidUserParameters(const std::vector<std::string> parameters)
{
	// <username> <hostname> <servername> <realname> <reallastname>

	// userInfo = alice * 192.168.1.10 :Alice Smith

	// alice: This is the username of the new user.
	// *: This is typically an unused parameter in modern IRC implementations, similar to the previous example.
	// 192.168.1.10: This is the hostname or IP address of the new user.
	// :Alice Smith: This is the realname of the new user.


	if (parameters.size() != EXPECTED_USER_PARAM)
		return (false);

	if (parameters[3][0] != ':')
		return (false);

	return (true);
}