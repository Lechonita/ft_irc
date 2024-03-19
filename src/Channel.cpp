#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Channel::Channel(const std::string& name, Client *client): _channelName(name)
{
	const channelClient newClient = {.client = client, .isOperator = true};
	_channelClients.push_back(newClient);
	// client->newChannel(*this);
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

	for (size_t i = 0; i < _channelClients.size(); i++)
	{
		// std::cout << RED << "client du channel= " << _channelClients[i].client->getClientNickname() << ", address = ";
		// std::cout << this << " client nouveau= " << client.getClientNickname() << ", address = " << &client << NC << std::endl;
		std::cout << RED << "i = " << i << "size = " << _channelClients.size() << NC << std::endl;
		if (_channelClients[i].client == &client)
			return ;
	}
	if (_channelPass.empty() == false)
	{
		if (passwrd == _channelPass)
		{
			_channelClients.push_back(newClient);
			client.newChannel(*this);
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
		client.newChannel(*this);
		// send message new client in channel
		//RPL_NAMREPLY
		//RPL_ENDOFNAMES
	}
}



void	Channel::removeClient(Client &client)
{
	std::vector<channelClient>::iterator	it;

	for (it = _channelClients.begin(); it < _channelClients.end(); it++)
	{
		if (&client == (*it).client)
		{
			_channelClients.erase(it);
		}
	}
}

void	Channel::printClients()
{
	for (size_t pos = 0; pos < _channelClients.size(); pos++)
	{
		std::cout << "	client " << pos << "= " << _channelClients[pos].client->getClientNickname()
		<< ", address = " << (_channelClients[pos].client) << std::endl;
	}
}




void	Channel::sendMessageToAll(std::string message)
{
	for(size_t i = 0; i < _channelClients.size(); i++)
	{
		Utils::sendMessage(message, *_channelClients[i].client);
	}
}
