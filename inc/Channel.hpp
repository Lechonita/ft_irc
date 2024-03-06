#ifndef CHANNEL_HPP
# define CHANNEL_HPP


#include "../inc/Client.hpp"
#include "../inc/defines.hpp"


class Client;


struct channelClient
{
	const Client	*client;
	bool			isOperator;
};


enum channelModes
{
	MODE_I,
	MODE_T,
	MODE_K,
	MODE_O,
	MODE_L
};


class Channel
{

	public:
		Channel(const std::string& name, const Client *const client);
		~Channel();


	private:

		std::string							_channelName;
		std::string							_channelTopic;
		std::vector<channelModes>			_channelModes;
		std::vector<channelClient>			_channelClients;
};

#endif