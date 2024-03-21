#ifndef CHANNEL_HPP
# define CHANNEL_HPP


#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"
#include "../inc/defines.hpp"


class Client;

class Server;

struct channelClient
{
	Client	*client;
	bool	isOperator;
};


// enum channelModes
// {
// 	MODE_I,
// 	MODE_T,
// 	MODE_K,
// 	MODE_O,
// 	MODE_L
// };


class Channel
{

	public:
		Channel(const std::string& name, Client *client);
		~Channel();

		// getters
		std::string					getChannelName() const;
		std::string					getChannelTopic() const;
		std::string					getChannelPass() const;
		// std::vector<channelModes>	getChannelModes() const;
		std::vector<channelClient>	getChannelClients() const;
		bool						getIMode() const {return (_iMode);}
		bool						getTMode() const {return (_tMode);}
		bool						getKMode() const {return (_kMode);}
		bool						getOMode() const {return (_oMode);}
		bool						getLMode() const {return (_lMode);}

		// setters
		void						setIMode(bool status);
		void						setTMode(bool status);
		void						setKMode(bool status);
		void						setOMode(bool status);
		void						setLMode(bool status);

		// utils
		void						newClient(std::string passwrd, Client &client);
		void						removeClient(Client &client);
		void						sendMessageToAll(std::string message);
		void						kickThoseMfOut(Client &client, Server &server, std::vector<std::string> clients, std::string message);
		bool						kickerIsQualified(Client &client);
		void						printClients();

	private:

		std::string					_channelName;
		std::string					_channelTopic;
		std::string					_channelPass;
		// std::vector<channelModes>	_channelModes;
		bool						_iMode;
		bool						_tMode;
		bool						_kMode;
		bool						_oMode;
		bool						_lMode;
		std::vector<channelClient>	_channelClients;
};

#endif