#include "Server.hpp"
#include "defines.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << RED << "Error: Invalid number of arguments" << NC << std::endl;
		return (EXIT_FAILURE);
	}
	(void)ac;
	int	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		return (EXIT_FAILURE);
	}

	// if (valid_port() == false)
	// { return (ERROR); }

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(av[1]));
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(mySocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		std::cerr << "Error: socket binding failed" << std::endl;
		close(mySocket);
		return (EXIT_FAILURE);
	}

	if (listen(mySocket, 10) == -1)
	{
		std::cerr << "Error listening on socket\n";
		close(mySocket);
		return EXIT_FAILURE;
	}

	while (true)
	{
		int	clientSocket = accept(mySocket, NULL, NULL);
		if (clientSocket == -1)
		{
			std::cerr << "Error accepting connection\n";
			continue;
		}
		const char* message = "Hello from the server!";
		send(clientSocket, message, strlen(message), 0);

		// close(clientSocket);
	}

	close(mySocket);

	return (EXIT_SUCCESS);
}
