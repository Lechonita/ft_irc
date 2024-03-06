#ifndef DEFINES_HPP
# define DEFINES_HPP


# include <map>
# include <vector>
# include <poll.h>
# include <sstream>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <iostream>
# include <exception>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>


// #ifdef DEBUG
// # define debug(msg) std::cerr << "DEBUG: " << msg << std::endl
// # else
// # define debug(msg)
// #endif


// Colors

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define ORANGE "\033[0;33m"
# define NC "\033[0m"

// Values

# define ARG_NB 3
# define ERROR -1
# define MAX_CLIENTS 10
# define TIMEOUT 10000
# define BUFFERSIZE 1024
# define NOT_WHITESPACE 0
# define EQUAL 0
# define EMPTY ""
# define MAX_NICK_LEN 9
# define CONNECTED true
# define DISCONNECTED false

// SERVER Error messages

# define ERR_SERVER "\033[0;31mDefault Server Exception\n\033[0m"
# define ERR_SERVER_SOCKET "\033[0;31mError: Invalid socket at server creation.\n\033[0m"
# define ERR_SERVER_BIND "\033[0;31mError: failed to bind port.\n\033[0m"
# define ERR_SERVER_LISTEN "\033[0;31mError: Failed to listen to socket.\n\033[0m"
# define ERR_SERVER_CONNECT "\033[0;31mError: Failed to grab connection.\n\033[0m"
# define ERR_SERVER_POLL "\033[0;31mError: Bad file descriptor.\n\033[0m"
# define ERR_SERVER_BLOCK "\033[0;31mError: Could not set server I/O operations to non-blocking.\n\033[0m"
# define ERR_SERVER_ACCEPT "\033[0;31mError: Could not connect new client.\n\033[0m"


// COMMANDS Error messages

# define PASS_NOT_ENTERED "\033[0;33m> Please use PASS command to enter server password first <\n\033[0m"
# define PASS_ALREADY_ENTERED "\033[0;33m> Password has already been entered <\n\033[0m"
# define ERR_NEEDMOREPARAMS "\033[0;33m461: Not enough parameters.\n\033[0m"
# define ERR_PASSWDMISMATCH "\033[0;33m464: Password incorrect.\n\033[0m"
# define ERR_UNKNOWNCOMMAND "\033[0;33m421: Unknown command.\n\033[0m"


// COMMANDS Messages

# define PASS_OK "\033[0;32mPassword correct !\033[0m Please set up Nickname now (use NICK command).\n"
# define NICK_OK "\033[0;32mNickname has been set !\033[0m Please set up Username now (use USER command).\n"
# define NICK_CHANGED "\033[0;32mYour nickname has been changed successfuly.\n\033[0m"

// CLIENT Error messages

# define ERR_CLIENT "\033[0;31mDefault Client Exception\n\033[0m"
# define ERR_CLIENT_BLOCK "\033[0;31mError: Could not set client I/O operations to non-blocking.\n\033[0m"


// Perror messages

# define PERR_SEND "send: "

// Constructor messages

// # define SERVER_C ""

#endif
