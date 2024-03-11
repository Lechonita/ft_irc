#include "../inc/Commands.hpp"

bool		Commands::isIrssi(const std::string& line)
{
	const std::vector<std::string>	parameters = Utils::splitParameters(line);

	if (parameters.size() > 0 && parameters[0] == "LS")
		return (true);
	return (false);
}
