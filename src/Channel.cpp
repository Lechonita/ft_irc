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
// std::vector<channelModes>	Channel::getChannelModes() const {return (_channelModes);}
std::vector<channelClient>	Channel::getChannelClients() const {return (_channelClients);}


void	Channel::setIMode(bool status) {_iMode = status;}
void	Channel::setTMode(bool status) {_tMode = status;}
void	Channel::setKMode(bool status) {_kMode = status;}
void	Channel::setOMode(bool status) {_oMode = status;}
void	Channel::setLMode(bool status) {_lMode = status;}


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



static void	errorUserNotInChan(Client &client,Server &server, std::string client_tokick, std::string channel_name)
{
	std::map<int, Client>	existingClients = server.getClientMap();
	std::map<int, Client>::iterator it;

	for (it = existingClients.begin(); it != existingClients.end(); it++)
	{
		if (it->second.getClientNickname() == client_tokick)
		{
			Utils::sendErrorMessage(ERR_USERNOTINCHANNEL, client, channel_name);
			break ;
		}
	}
	if (it == existingClients.end())
		Utils::sendErrorMessage(ERR_NOSUCHNICK, client);
}



void	Channel::kickThoseMfOut(Client &client, Server &server, std::vector<std::string> clients, std::string message)
{
	if (kickerIsQualified(client) == false)
		return ;

	for (size_t pos_client = 0; pos_client < clients.size(); pos_client++)
	{
		std::vector<channelClient>::iterator	it= _channelClients.begin();
		size_t pos_chan;

		client.setLastArgument(clients[pos_client]);
		for (pos_chan = 0; pos_chan <= _channelClients.size(); pos_chan++)
		{
			if (pos_chan == _channelClients.size())
			{
				errorUserNotInChan(client, server, clients[pos_client], _channelName);
				break ;
			}
			else if (it->client->getClientNickname() == clients[pos_client])
			{
				Utils::kickMessageSuccessfull(client, server, _channelName, message, clients[pos_client]);
				it->client->removeChannelFromClient(*this);
				_channelClients.erase(it);
				break ;
			}
			it++;
		}
	}
}