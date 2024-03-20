#include "../inc/Server.hpp"

void	Server::signalExit(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
	{
		doSignal = true;
		std::cout << SIGNAL_EXIT << std::endl;
	}
}
