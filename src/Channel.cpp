#include "../inc/Channel.hpp"
#include "../inc/defines.hpp"


/********************************************************************************/
/*************************** CONSTRUCTOR / DESTRUCTOR ***************************/
/********************************************************************************/

Channel::Channel(const std::string& name, Client *client): _channelName(name), _channelTopic(EMPTY), _channelPass(EMPTY)
{
	const channelClient newClient = {.client = client, .isOperator = true};
	_channelClients.push_back(newClient);
	_iMode = false;
	_tMode = true; //set to true from the beginning since TOPIC is an operator command in the subject
	_kMode = false;
	_lMode = false;
}



Channel::~Channel() {}

/********************************************************************************/
/***************************** MEMBER FUNCTIONS *********************************/
/********************************************************************************/


// Setters

void		Channel::setChannelTopic(const std::string& channelTopic) { _channelTopic = channelTopic; }



// Getters

std::string					Channel::getChannelName() const {return (_channelName);}
std::string					Channel::getChannelTopic() const {return (_channelTopic);}
std::string					Channel::getChannelPass() const {return (_channelPass);}
std::vector<channelClient>	Channel::getChannelClients() const {return (_channelClients);}


void	Channel::setIMode(bool status) {_iMode = status;}
void	Channel::setTMode(bool status) {_tMode = status;}
void	Channel::setKMode(bool status) {_kMode = status;}
void	Channel::setLMode(bool status) {_lMode = status;}
void	Channel::setPassword(std::string password) {_channelPass = password;}
void	Channel::setTopic(std::string topic) {_channelTopic = topic;}

size_t	Channel::setUserLimit(std::string limit)
{
	double limint = strtod(limit.c_str(), NULL);

	if (limint < INT_MAX && limint > 0)
	{
		_usersLimit = atoi(limit.c_str());
		return (GOOD_LIMIT);
	}
	return (BAD_LIMIT);
}



void	Channel::newClient(Client &client)
{
	channelClient	newClient = {.client = &client, .isOperator = false};

	for (size_t i = 0; i < _channelClients.size(); i++)
	{
		std::cout << "size = " << _channelClients.size() << " i = " << i << std::endl;
		if (_channelClients[i].client == &client)
			return ;
	}
	_channelClients.push_back(newClient);
	client.newChannel(*this);
}



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
		}
		else
		{
			Utils::sendFormattedMessage(ERR_BADCHANNELKEY, client, _channelName);
		}
	}
	else
	{
		_channelClients.push_back(newClient);
		client.newChannel(*this);
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



bool	Channel::clientIsInChan(Client &client)
{
	for (size_t pos = 0; pos < _channelClients.size(); pos++)
	{
		if (client.getClientNickname() == _channelClients[pos].client->getClientNickname())
			return (true);
	}
	return (false);
}



void	Channel::sendPrivmsgToChan(const Client& client, std::string message)
{
	for(size_t i = 0; i < _channelClients.size(); i++)
	{
		if (client.getClientNickname() != _channelClients[i].client->getClientNickname())
			Utils::sendMessage(message, *_channelClients[i].client);
	}
}



void	Channel::sendMessageToAll(std::string message)
{
	for(size_t i = 0; i < _channelClients.size(); i++)
	{
		Utils::sendMessage(message, *_channelClients[i].client);
	}
}


void	Channel::sendMessageToAll(std::string message, std::string parameter)
{
	if (parameter[0] == PREFIX_CHAN)
	{
		for(size_t i = 0; i < _channelClients.size(); i++)
		{
			Utils::sendFormattedMessage(message, *_channelClients[i].client, parameter);
		}
	}
	else
	{
		for(size_t i = 0; i < _channelClients.size(); i++)
		{
			Utils::sendMessage(parameter + message, *_channelClients[i].client);
		}
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
		Utils::sendFormattedMessage(ERR_NOTONCHANNEL, client, _channelName); //The error sent by default is ERR_CHANOPRIVSNEEDED, we chose to sent this instead for clarity
		return (false);
	}
	else if (_channelClients[pos].isOperator == false)
	{
		Utils::sendFormattedMessage(ERR_CHANOPRIVSNEEDED, client, _channelName);
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
			Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client, channel_name);
			break ;
		}
	}
	if (it == existingClients.end())
		Utils::sendFormattedMessage(ERR_NOSUCHNICK, client);
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
		if (clients[pos_client] != client.getClientNickname())
		{
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
		else
			Utils::sendFormattedMessage("503 <client> cannot kick self from <channelName>", client, _channelName);
	}
}


bool	Channel::isChanOp(const Client &client)
{
	for (size_t i = 0; i < _channelClients.size(); i++)
	{
		if (_channelClients[i].client == &client)
		{
			bool status;
			_channelClients[i].isOperator == true ? status = true : status = false;
			return (status);
		}
	}
	return (false);
}



static channelModes	findModeToChange(char mode)
{
	switch (mode)
	{
		case 'k':
			return (MODE_K);

		case 'o':
			return (MODE_O);

		case 'l':
			return (MODE_L);

		case 't':
			return (MODE_T);

		case 'i':
			return (MODE_I);
	}
	return (DEFAULT);
}



void	Channel::setSimpleModes(std::vector<std::string> modes_without_args)
{
	for (size_t pos = 0; pos < modes_without_args.size(); pos++)
	{
		channelModes mode = findModeToChange(modes_without_args[pos][1]);

		if (modes_without_args[pos][0] == '+')
		{
			switch (mode)
			{
				case MODE_T:
					setTMode(true);
					break;
				case MODE_I:
					setIMode(true);
					break;
				default :
					break;
			}
		}
		else
		{
			switch (mode)
			{
				case MODE_T:
					setTMode(false);
					break;
				case MODE_I:
					setIMode(false);
					break;
				default :
					break;
			}
		}
	}
}



int	Channel::giveOpStatusToClient(Client& client, std::string client_name)
{
	for (size_t pos = 0; pos < _channelClients.size(); pos++)
	{
		if (_channelClients[pos].client->getClientNickname() == client_name)
		{
			_channelClients[pos].isOperator = true;
			return (CLIENT_FOUND);
		}
	}
	client.setLastArgument(client_name);
	Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client, _channelName);
	return (CLIENT_NOT_FOUND);
}



int	Channel::takeOpStatusFromClient(Client& client, std::string client_name)
{
	for (size_t pos = 0; pos < _channelClients.size(); pos++)
	{
		if (_channelClients[pos].client->getClientNickname() == client_name)
		{
			_channelClients[pos].isOperator = false;
			return (CLIENT_FOUND);
		}
	}
	client.setLastArgument(client_name);
	Utils::sendFormattedMessage(ERR_USERNOTINCHANNEL, client, _channelName);
	return (CLIENT_NOT_FOUND);
}



void	Channel::setArgModes(Client& client, std::vector<std::string> &modes_args, std::vector<std::string> &modes_with_args)
{
	for (size_t pos = 0; pos < modes_with_args.size(); pos++)
	{
		channelModes mode = findModeToChange(modes_with_args[pos][1]);

		if (modes_with_args[pos][0] == '+')
		{
			if (pos < modes_args.size())
			{
				switch (mode)
				{
					case MODE_K:
						std::cout << BLUE << "pwd = " << modes_args[pos] << NC << std::endl;
						setPassword(modes_args[pos]);
						setKMode(true);
						break;

					case MODE_O:
						if (giveOpStatusToClient(client, modes_args[pos]) != CLIENT_FOUND)
						{
							modes_with_args.erase(modes_with_args.begin() + pos);
							modes_args.erase(modes_args.begin() + pos);
							pos--;
						}
						break;

					case MODE_L:
						if (setUserLimit(modes_args[pos]) == GOOD_LIMIT)
							setLMode(true);
						else
						{
							modes_with_args.erase(modes_with_args.begin() + pos);
							modes_args.erase(modes_args.begin() + pos);
							pos--;
						}
						break;
					default :
						break;
				}
			}
			else
			{
				modes_with_args.erase(modes_with_args.begin() + pos);
				pos--;
			}
		}
		else
		{
			switch (mode)
			{
				case MODE_K:
					setPassword("");
					setKMode(false);
					break;

				case MODE_O:
					if (pos < modes_args.size())
					{
						if (takeOpStatusFromClient(client, modes_args[pos]) != CLIENT_FOUND)
						{
							modes_with_args.erase(modes_with_args.begin() + pos);
							modes_args.erase(modes_args.begin() + pos);
							pos--;
						}
					}
					else
					{
						modes_with_args.erase(modes_with_args.begin() + pos);
						pos--;
					}
					break;

				case MODE_L:
					setLMode(false);
					break;
				default :
					break;
			}
		}
	}
}
