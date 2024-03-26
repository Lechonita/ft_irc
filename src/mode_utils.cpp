/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:33:45 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/25 16:31:29 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

static std::vector<std::string>	extractChannels(std::string mode_params, size_t &pos)
{
	std::vector<std::string>	channels;

	while (isspace(mode_params[pos]) != NOT_WHITESPACE)
		pos++;

	while (pos < mode_params.size())
	{
		int	start = pos;

		while (mode_params[pos] != '\0' && mode_params[pos] != ',' && isspace(mode_params[pos]) == NOT_WHITESPACE)
		{
			pos++;
		}
		std::string	chan = mode_params.substr(start, pos - start);

		channels.push_back(chan);
		if (isspace(mode_params[pos]) != NOT_WHITESPACE || pos == mode_params.size())
			break ;
		pos++;
	}
	return (channels);
}


//	je dois d'abord recevoir un signe + ou -
//	lire jusqu'au prochain whitespace
//	ajouter chaque char aux modes

static int	removeOrAddMode(char mode, size_t &i)
{
	if (mode == '+')
	{
		i++;
		return (ADD_MODE);
	}
	else if (mode == '-')
	{
		i++;
		return (REMOVE_MODE);
	}
	return (NO_MODE);
}



static bool	isArgMode(char mode)
{
	std::vector<char>	arg_mode;

	arg_mode.push_back('k');
	arg_mode.push_back('o');
	arg_mode.push_back('l');
	for (size_t pos = 0; pos < arg_mode.size(); pos++)
	{
		if (mode == arg_mode[pos])
			return (true);
	}
	return (false);
}



static bool	isSimpleMode(char mode)
{
	std::vector<char>	simple_mode;

	simple_mode.push_back('t');
	simple_mode.push_back('i');
	for (size_t pos = 0; pos < simple_mode.size(); pos++)
	{
		if (mode == simple_mode[pos])
			return (true);
	}
	return (false);
}



static bool	modeAlreadyInList(char mode, std::vector<std::string> *modes)
{
	for (size_t pos = 0; pos < modes->size(); pos++)
	{
		if ((*modes)[pos][0] == mode)
			return (true);
	}
	return (false);
}



static int	whatKindOfMode(std::vector<std::string> *modes_with_args, std::vector<std::string> *modes_without_args, std::string modes, Client& client)
{
	size_t				i = 0;
	char				mode = modes[i];
	int					sign = removeOrAddMode(mode, i);
	const char *test = modes.c_str();
	std::cout << test << std::endl;
	if (sign == NO_MODE)
		return (sign);
	while (i < modes.size())
	{
		mode = modes[i];
		std::string	full_mode(1, mode);
		if (isArgMode(mode) == true)
		{
			if (sign == ADD_MODE)
				full_mode = "+" + full_mode;
			else if (sign == REMOVE_MODE)
				full_mode = "-" + full_mode;
			if (modeAlreadyInList(mode, modes_with_args) == false)
				modes_with_args->push_back(full_mode);
		}
		else if (isSimpleMode(mode) == true)
		{
			if (sign == ADD_MODE)
				full_mode = "+" + full_mode;
			else if (sign == REMOVE_MODE)
				full_mode = "-" + full_mode;
			if (modeAlreadyInList(mode, modes_without_args) == false)
				modes_without_args->push_back(full_mode);
		}
		else if (modes[i] == '-' || modes[i] == '+')
		{
			modes[i] == '+' ? sign = ADD_MODE : sign = REMOVE_MODE;
		}
		else
		{
			std::string unknown_mode(1, modes[i]);
			client.setLastArgument(unknown_mode);
			Utils::sendErrorMessage(ERR_UNKNOWNMODE, client);
		}
		i++;
	}
	return (sign);
}



static void	extractModesNArgs(std::string mode_params, std::vector<std::string> *modes_with_args, std::vector<std::string> *modes_without_args, std::vector<std::string> *modes_args, size_t &pos, Client& client)
{
	while (isspace(mode_params[pos]) != NOT_WHITESPACE)
		pos++;
	while (pos < mode_params.size())
	{
		int	start = pos;

		while (isspace(mode_params[pos]) == NOT_WHITESPACE && pos != mode_params.size())
			pos++;

		std::string	modes = mode_params.substr(start, pos - start);
	// 	const char *test = modes.c_str();
	// std::cout << test << std::endl;
		if (whatKindOfMode(modes_with_args, modes_without_args, modes, client) == NO_MODE)
		{
			modes_args->push_back(modes);
		}
		if (pos == mode_params.size())
			break ;
		pos++;
	}
}



void	Commands::checkModeParams(std::string mode_params, std::vector<std::string> *channels, std::vector<std::string> *modes_with_args, std::vector<std::string> *modes_without_args, std::vector<std::string> *modes_args, Client& client)
{
	size_t	pos = 0;

	*channels = extractChannels(mode_params, pos);
	extractModesNArgs(mode_params, modes_with_args, modes_without_args, modes_args, pos, client);
}
