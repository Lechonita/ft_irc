/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrouillo <jrouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:55:12 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/13 11:52:36 by jrouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

static std::vector<std::string>	extractChannels(std::string join_params, int &pos)
{
	std::vector<std::string>	channels;

	while (isspace(join_params[pos]) != NOT_WHITESPACE)
		pos++;

	while (join_params[pos] != '\0')
	{
		int	start = pos;

		while (join_params[pos] != '\0' && join_params[pos] != ',' && isspace(join_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	chan = join_params.substr(start, pos - start);

		channels.push_back(chan);
		if (isspace(join_params[pos]) != NOT_WHITESPACE || join_params[pos] == '\0')
			break ;
		pos++;
	}
	return (channels);
}



static std::vector<std::string>	extractPasswrds(std::string join_params, int &pos)
{
	std::vector<std::string>	passwrd;

	while (isspace(join_params[pos]) != NOT_WHITESPACE && join_params[pos] != ',')
		pos++;

	while (join_params[pos] != '\0')
	{
		int	start = pos;

		while (join_params[pos] != '\0' && join_params[pos] != ',' && isspace(join_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	pass = join_params.substr(start, pos - start);

		if (pass.empty() == false)
			passwrd.push_back(pass);
		if (isspace(join_params[pos]) != NOT_WHITESPACE || join_params[pos] == '\0')
			break ;
		pos++;
	}
	return (passwrd);
}



void		Commands::checkJoinParams(std::string join_params, std::vector<std::string> *channels, std::vector<std::string> *passwrds)
{
	int	pos = 0;

	*channels = extractChannels(join_params, pos);
	*passwrds = extractPasswrds(join_params, pos);
}