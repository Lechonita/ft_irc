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
		std::cout << RED << "	client " << pos << "= " << _channelClients[pos].client->getClientNickname()
		<< ", address = " << (_channelClients[pos].client) << NC << std::endl;
	}
}




void	Channel::sendMessageToAll(std::string message)
{
	for(size_t i = 0; i < _channelClients.size(); i++)
	{
		Utils::sendMessage(message, *_channelClients[i].client);
	}
}



bool	Channel::kickerIsQualified(Client &client)
{
	size_t	pos;

	for (pos = 0; pos < _channelClients.size(); pos++)
	{
		if (_channelClients[pos].client == &client)
			break ;
	}
	if (pos == _channelClients.size())
	{
		Utils::sendErrorMessage(ERR_NOTONCHANNEL, client, _channelName); //The error sent by default is ERR_CHANOPRIVSNEEDED, we chose to sent this instead for clarity
		return (false);
	}
	else if (_channelClients[pos].isOperator == false)
	{
		Utils::sendErrorMessage(ERR_CHANOPRIVSNEEDED, client, _channelName);
		return (false);
	}
	return (true);
}



void	Channel::kickThoseMfOut(Client &client, Server &server, std::vector<std::string> clients, std::string message)
{
	if (kickerIsQualified(client) == false)
		return ;

	for (size_t i = 0; i < clients.size(); i++)
	{
		std::vector<channelClient>::iterator	it;

		for (it = _channelClients.begin(); it != _channelClients.end(); it++)
		{
			if (it->client->getClientNickname() == clients[i])
			{
				it->client->removeChannelFromClient();
				_channelClients.erase(it);
			}
		}
		if (it == _channelClients.end())
		{
			// faire fonction qui cherche client et affiche message d'erreur
			std::map<int, Client>	existingClients = server.getClientMap();

			std::map<int, Client>::iterator it = ;
			Utils::sendErrorMessage(ERR_USERNOTINCHANNEL, clients[i], _channelName);
		}
	}
}