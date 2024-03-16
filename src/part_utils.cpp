/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernaze <cbernaze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:02:17 by cbernaze          #+#    #+#             */
/*   Updated: 2024/03/16 18:04:59 by cbernaze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

void		Commands::checkPartParams(std::string part_params, std::vector<std::string> *channels)
{
	int	pos = 0;

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

		(*channels).push_back(chan);
		if (isspace(part_params[pos]) != NOT_WHITESPACE || part_params[pos] == '\0')
			break ;
		pos++;
	}
}