/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:46:00 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/20 15:51:14 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"


static std::vector<std::string>	extractChannels(std::string kick_params, int &pos)
{
	std::vector<std::string>	channels;

	while (isspace(kick_params[pos]) != NOT_WHITESPACE)
		pos++;

	while (kick_params[pos] != '\0')
	{
		int	start = pos;

		while (kick_params[pos] != '\0' && kick_params[pos] != ',' && isspace(kick_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	chan = kick_params.substr(start, pos - start);

		channels.push_back(chan);
		if (isspace(kick_params[pos]) != NOT_WHITESPACE || kick_params[pos] == '\0')
			break ;
		pos++;
	}
	return (channels);
}


static std::vector<std::string>	extractClients(std::string kick_params, int &pos)
{
	std::vector<std::string>	clients;

	while (isspace(kick_params[pos]) != NOT_WHITESPACE && kick_params[pos] != ',')
		pos++;

	while (kick_params[pos] != '\0')
	{
		int	start = pos;

		while (kick_params[pos] != '\0' && kick_params[pos] != ',' && isspace(kick_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	client = kick_params.substr(start, pos - start);

		if (client.empty() == false)
			clients.push_back(client);
		if (isspace(kick_params[pos]) != NOT_WHITESPACE || kick_params[pos] == '\0')
			break ;
		pos++;
	}
	return (clients);
}


static std::string	getMessage(std::string parameters, int &pos)
{
	std::string	message;

	while (isspace(parameters[pos]) != NOT_WHITESPACE)
		pos++;

	message = parameters.substr(pos);
	if (message[0] == ':')
		message.erase(0, 1);
	return (message);
}


void		Commands::checkKickParams(std::string kick_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds, std::string *message)
{
	int	pos = 0;

	*channels = extractChannels(kick_params, pos);
	*passwrds = extractClients(kick_params, pos);
	*message = getMessage(kick_params, pos);
}