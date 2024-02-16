#include "Server.hpp"
#include "defines.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << RED << "Error: Invalid number of arguments" << NC << std::endl;
		return (EXIT_FAILURE);
	}

	(void)av;
	return (EXIT_SUCCESS);
}
