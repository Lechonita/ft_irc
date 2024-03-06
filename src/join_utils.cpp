/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:55:12 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/06 19:09:22 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"
#include "../inc/Utils.hpp"

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

		if (chan[0] != '#')
			std::cout << Utils::sendErrorMsg(ERR_NOSUCHCHANNEL) << std::endl;
		else
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
		std::string	chan = join_params.substr(start, pos - start);

		if (chan.empty() == false)
			passwrd.push_back(chan);
		if (isspace(join_params[pos]) != NOT_WHITESPACE || join_params[pos] == '\0')
			break ;
		pos++;
	}
	return (passwrd);
}



bool		Commands::checkJoinParams(std::string join_params)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	passwrds;
	int							pos = 0;

	channels = extractChannels(join_params, pos);
	passwrds = extractPasswrds(join_params, pos);

	for (size_t i = 0; i < channels.size(); i++)
		std::cout << RED << "chan: " << channels[i] << NC << std::endl;
	for (size_t i = 0; i < passwrds.size(); i++)
		std::cout << GREEN << "pwd: " << passwrds[i] << NC << std::endl;
	return (true);
}