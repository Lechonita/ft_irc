#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Channel::Channel(const std::string& name, const Client *const client): _channelName(name)
{
	const channelClient newClient = {.client = client, .isOperator = true};
	_channelClients.push_back(newClient);
	// send message new client in channel
	//RPL_NAMREPLY
	//RPL_ENDOFNAMES
}



Channel::~Channel() {}

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

	for (size_t i = 0; i <= _channelClients.size(); i++)
	{
		if (_channelClients[i].client == &client)
			return ;
	}
	if (_channelPass.empty() == false)
	{
		if (passwrd == _channelPass)
		{
			_channelClients.push_back(newClient);
			// send message new client in channel
			//RPL_NAMREPLY
			//RPL_ENDOFNAMES
		}
		else
		{
			Utils::sendErrorMessage(ERR_BADCHANNELKEY, client, _channelName);
		}
	}
	else
	{
		_channelClients.push_back(newClient);
		// send message new client in channel
		//RPL_NAMREPLY
		//RPL_ENDOFNAMES
	}
}