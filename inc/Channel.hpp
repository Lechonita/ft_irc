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
		Channel(const std::string& name, Client *client);
		~Channel();

		// getters
		std::string					getChannelName() const;
		std::string					getChannelTopic() const;
		std::string					getChannelPass() const;
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
		void						setPassword(std::string password);
		void						setPassword(std::string topic);
		void						setUserLimit(std::string limit);

		// utils
		void						newClient(std::string passwrd, Client &client);
		void						removeClient(Client &client);
		void						sendMessageToAll(std::string message);
		void						kickThoseMfOut(Client &client, Server &server, std::vector<std::string> clients, std::string message);
		void						setSimpleModes(const Client& client, std::vector<std::string> modes_without_args);
		void						setArgModes(Client& client, std::vector<std::string> modes_args, std::vector<std::string> modes_with_args);
		void						printClients();
		bool						kickerIsQualified(Client &client);
		bool						isChanOp(const Client &client);
		void						giveOpStatusToClient(Client& client, std::string client_name);
		void						takeOpStatusFromClient(Client& client, std::string client_name);

	private:

		std::string					_channelName;
		std::string					_channelTopic;
		std::string					_channelPass;
		bool						_iMode;
		bool						_tMode;
		bool						_kMode;
		bool						_oMode;
		bool						_lMode;
		std::vector<channelClient>	_channelClients;
		int							_usersLimit;
};

#endif