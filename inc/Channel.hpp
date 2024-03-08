#ifndef CHANNEL_HPP
# define CHANNEL_HPP


#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"
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
		std::string					getChannelName() const;
		std::string					getChannelTopic() const;
		std::string					getChannelPass() const;
		std::vector<channelModes>	getChannelModes() const;
		std::vector<channelClient>	getChannelClients() const;
		void						newClient(std::string passwrd, Client &client);


	private:

		std::string					_channelName;
		std::string					_channelTopic;
		std::string					_channelPass;
		std::vector<channelModes>	_channelModes;
		std::vector<channelClient>	_channelClients;
};

#endif