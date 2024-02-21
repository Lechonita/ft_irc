#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <sys/socket.h> // socket
# include <netinet/in.h>
# include <unistd.h>

class Server
{

public:
	Server();
	Server(const Server& copy);
	~Server();

	Server& operator=(const Server& src);

private:

	const int	_socketfd; // file descriptor that can be used for communication
	const int	_bind; // assign an IP address and port to the socket
	const int	_listen; // marks a socket as passive - used to accept connections
	const int	_acceptfd; // extracts an element from a queue of connections (The queue created by listen) for a socket
};

#endif