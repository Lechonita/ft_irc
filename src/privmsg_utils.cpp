/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:24:43 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/20 15:17:24 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

static std::vector<std::string>	getReceiver(std::string parameters, int &pos)
{
	std::vector<std::string>	receivers;

	while (isspace(parameters[pos]) != NOT_WHITESPACE)
		pos++;

	while (parameters[pos] != '\0')
	{
		int	start = pos;

		while (parameters[pos] != '\0' && parameters[pos] != ',' && isspace(parameters[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	receiver = parameters.substr(start, pos - start);

		if (receiver.empty() == false)
			receivers.push_back(receiver);
		if (isspace(parameters[pos]) != NOT_WHITESPACE || parameters[pos] == '\0')
			break ;
		pos++;
	}
	return (receivers);
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

void	Commands::checkPrivmsgParams(std::string parameters, std::vector<std::string> *receivers, std::string *message)
{
	int	pos = 0;

	*receivers = getReceiver(parameters, pos);
	*message = getMessage(parameters, pos);
}
