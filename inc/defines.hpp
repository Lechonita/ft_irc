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

# define NO_PASS "\033[0;33m> Please use PASS command to enter server password first <\n\033[0m"


// CLIENT Error messages

# define ERR_CLIENT "\033[0;31mDefault Client Exception\n\033[0m"
# define ERR_CLIENT_BLOCK "\033[0;31mError: Could not set client I/O operations to non-blocking.\n\033[0m"


// Perror messages

# define PERR_SEND "send: "

// Constructor messages

// # define SERVER_C ""

// IRC Error messages

# define ERR_NOSUCHNICK		"401 <client> <arg> :No such nick/channel"
# define ERR_NOSUCHCHANNEL	"403 <client> <channelName> :No such channel"
# define ERR_CANNOTSENDTOCHAN \
	"404 <client> <channelName> :Cannot send to channel"
# define ERR_TOOMANYCHANNELS \
	"405 <client> <channelName> :You have joined too many channels"
# define ERR_NOTEXTTOSEND	 "412 <client> :No text to send"
# define ERR_UNKNOWNCOMMAND	 "421 <client> <command> :Unknown command"
# define ERR_NONICKNAMEGIVEN	 "431 <client> :No nickname given"
# define ERR_ERRONEUSNICKNAME "432 <client> <arg> :Erroneus nickname"
# define ERR_NICKNAMEINUSE	 "433 *<client> <arg> :Nickname is already in use"
# define ERR_USERNOTINCHANNEL \
	"441 <client> <arg> <channelName> :They aren't on that channel"
# define ERR_NOTONCHANNEL \
	"442 <client> <channelName> :You're not on that channel"
# define ERR_USERONCHANNEL \
	"443 <client> <arg> <channelName> :is already on channel"
# define ERR_NEEDMOREPARAMS	  "461 <client> <command> :Not enough parameters"
# define ERR_ALREADYREGISTERED "462 <client> :You may not reregister"
# define ERR_PASSWDMISMATCH	  "464 <client> :Password incorrect"
# define ERR_CHANNELISFULL	  "471 <client> <channelName> :Cannot join channel (+l)"
# define ERR_INVITEONLYCHAN \
	"473 <client> <channelName> :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY "475 <client> <channelName> :Cannot join channel (+k)"
# define ERR_BADCHANMASK	  "476 <channelName> :Bad Channel Mask"
# define ERR_CHANOPRIVSNEEDED \
	"482 <client> <channelName> :You're not channel operator"

#endif
