#include "../inc/Server.hpp"
#include "../inc/defines.hpp"
#include "../inc/Commands.hpp"


bool		Commands::areValidUserParameters(const std::string& userInfo, const Server& server)
{
	// USER alice * 192.168.1.10 :Alice Smith

	// USER: This is the command itself.
	// alice: This is the username of the new user.
	// *: This is typically an unused parameter in modern IRC implementations, similar to the previous example.
	// 192.168.1.10: This is the hostname or IP address of the new user.
	// :Alice Smith: This is the realname of the new user.
}
