/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:02:17 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/18 15:21:04 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

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

static std::vector<std::string>	extractChannels(std::string part_params, int &pos)
{
	std::vector<std::string>	channels;

	while (isspace(part_params[pos]) != NOT_WHITESPACE)
		pos++;

	while (part_params[pos] != '\0')
	{
		int	start = pos;

		while (part_params[pos] != '\0' && part_params[pos] != ',' && isspace(part_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	chan = part_params.substr(start, pos - start);

		channels.push_back(chan);
		if (isspace(part_params[pos]) != NOT_WHITESPACE || part_params[pos] == '\0')
			break ;
		pos++;
	}
	return (channels);
}

void		Commands::checkPartParams(std::string part_params, std::vector<std::string> *channels, std::string *message)
{
	int	pos = 0;

	*channels = extractChannels(part_params, pos);
	*message = getMessage(part_params, pos);
}
