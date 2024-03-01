#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Channel::Channel(const std::string& name, const Client *const client): _channelName(name), _channelMode(NO_MODE)
{
	const channelClient newClient = {.client = client, .isOperator = true};
	_clientsInside.push_back(newClient);
	(void)_channelMode;

	std::cout << "Client " << client->getClientUsername() << " has joined the channel " << name << std::endl;
}

Channel::~Channel()
{
	std::cout << "Default destructor." << std::endl;
}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

