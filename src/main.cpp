#include "../inc/Server.hpp"
#include "../inc/Signal.hpp"
#include "../inc/defines.hpp"

bool	doSignal = false;

static bool	isValidPort(const char *port)
{
	for(size_t i = 0; port[i]; ++i)
	{
		if (isdigit(port[i]) == false)
			return (false);
	}

	size_t	convertedPort = strtol(port, NULL, 10);
	if (convertedPort <= 1023 || convertedPort >= 65535)
		return (false);
	return (true);
}

int main(int ac, char **av)
{
	if (ac != ARG_NB)
	{
		std::cout << RED << "Error: Invalid number of arguments" << NC << std::endl;
		return (EXIT_FAILURE);
	}

	if (isValidPort(av[1]) == false)
	{
		std::cout << RED << "Error: Invalid port." << NC << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		std::string	port(av[1]);
		std::string	password(av[2]);
		Server server(port, password);

		std::signal(SIGINT, Server::signalExit);
		std::signal(SIGQUIT, Server::signalExit);

		while (doSignal == false)
		{
			server.runServer();
			// server.printAll();
		}
	}
	catch (Server::Exception &e)
	{
		std::cout << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
