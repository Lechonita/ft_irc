#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Channel::Channel(const std::string& name, const Client *const client): _channelName(name)
{
	const channelClient newClient = {.client = client, .isOperator = true};
	_channelClients.push_back(newClient);
	std::cout << "Client " << client->getClientUsername() << " has joined the channel " << name << std::endl;
}



Channel::~Channel()
{
	std::cout << "Default destructor." << std::endl;
}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/

std::string					Channel::getChannelName() const {return (_channelName);}
std::string					Channel::getChannelTopic() const {return (_channelTopic);}
std::string					Channel::getChannelPass() const {return (_channelPass);}
std::vector<channelModes>	Channel::getChannelModes() const {return (_channelModes);}
std::vector<channelClient>	Channel::getChannelClients() const {return (_channelClients);}



void	Channel::newClient(std::string passwrd, Client &client)
{
	channelClient	newClient = {.client = &client, .isOperator = false};

	if (_channelPass.empty() == false)
	{
		if (passwrd == _channelPass)
		{
			_channelClients.push_back(newClient);
			// send message new client in channel
		}
		else
		{
			Utils::sendErrorMessage(ERR_BADCHANNELKEY, NULL, NULL, client, _channelName.c_str());
		}
	}
	else
	{
		_channelClients.push_back(newClient);
		// send message new client in channel
	}
}