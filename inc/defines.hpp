#ifndef DEFINES_HPP
# define DEFINES_HPP


# include <map>
# include <ctime>
# include <vector>
# include <poll.h>
# include <cstdio>
# include <sstream>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
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
# define BLUE "\033[0;36m"
# define PINK "\033[0;35m"
# define NC "\033[0m"

// Values

# define ARG_NB 3
# define ERROR -1
# define MAX_CLIENTS 10
# define TIMEOUT 30000
# define BUFFERSIZE 1024
# define NOT_WHITESPACE 0
# define EQUAL 0
# define EMPTY ""
# define SPACE " "
# define MAX_NICK_LEN 9
# define PATTERN_COUNT 4
# define EXPECTED_USER_PARAM 5
# define INCOMING_MSG "\033[0;36m<< \033[0m"
# define OUTGOING_MSG "\033[0;35m>> \033[0m"
// # define CONNECTED true
// # define DISCONNECTED false


// Server specifications

# define HOST_NAME "localhost"
# define SERVER_NAME "IRCserver"

# define PREFIX_CHAN '#'
# define END_MSG "\r\n"


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

# define PASS_NOT_ENTERED "\033[0;33m> Please use PASS command to enter server password first <\033[0m"
# define NICK_NOT_ENTERED "\033[0;33m> Please set Nickname first (use NICK command) <\033[0m"
# define PASS_ALREADY_ENTERED "\033[0;33m> Password has already been entered <\033[0m"

# define ERR_NOSUCHNICK			"\033[0;33m401 <client> <arg> :No such nick/channel\033[0m"
# define ERR_NOSUCHCHANNEL		"\033[0;33m403 <client> <channelName> :No such channel\033[0m"
# define ERR_CANNOTSENDTOCHAN	"\033[0;33m404 <client> <channelName> :Cannot send to channel\033[0m"
# define ERR_TOOMANYCHANNELS	"\033[0;33m405 <client> <channelName> :You have joined too many channels\033[0m"
# define ERR_NOTEXTTOSEND		"\033[0;33m412 <client> :No text to send\033[0m"
# define ERR_UNKNOWNCOMMAND		"\033[0;33m421 <client> <command> :Unknown command\033[0m"
# define ERR_NONICKNAMEGIVEN	"\033[0;33m431 <client> :No nickname given\033[0m"
# define ERR_ERRONEUSNICKNAME	"\033[0;33m432 <client> <arg> :Erroneus nickname\033[0m"
# define ERR_NICKNAMEINUSE		"\033[0;33m433 *<client> <arg> :Nickname is already in use\033[0m"
# define ERR_USERNOTINCHANNEL	"\033[0;33m441 <client> <arg> <channelName> :They aren't on that channel\033[0m"
# define ERR_NOTONCHANNEL		"\033[0;33m442 <client> <channelName> :You're not on that channel\033[0m"
# define ERR_USERONCHANNEL		"\033[0;33m443 <client> <arg> <channelName> :is already on channel\033[0m"
# define ERR_NEEDMOREPARAMS		"\033[0;33m461 <client> <command> :Not enough parameters\033[0m"
# define ERR_ALREADYREGISTERED	"\033[0;33m462 <client> :You may not reregister\033[0m"
# define ERR_PASSWDMISMATCH		"\033[0;33m464 :Password incorrect\033[0m"
# define ERR_CHANNELISFULL		"\033[0;33m471 <client> <channelName> :Cannot join channel (+l)\033[0m"
# define ERR_INVITEONLYCHAN		"\033[0;33m473 <client> <channelName> :Cannot join channel (+i)\033[0m"
# define ERR_BADCHANNELKEY		"\033[0;33m475 <client> <channelName> :Cannot join channel (+k)\033[0m"
# define ERR_BADCHANMASK		"\033[0;33m476 <channelName> :Bad Channel Mask\033[0m"
# define ERR_CHANOPRIVSNEEDED	"\033[0;33m482 <client> <channelName> :You're not channel operator\033[0m"


//Replies

# define RPL_TOPIC 0
# define RPL_NOTOPIC 0
# define RPL_CHANNELMODEIS 0
# define RPL_BANLIST 0
# define RPL_EXCEPTLIST 0
# define RPL_INVITELIST 0
# define RPL_UNIQOPIS 0
# define RPL_ENDOFBANLIST 0
# define RPL_ENDOFEXCEPTLIST 0
# define RPL_ENDOFINVITELIST 0
# define RPL_NAMREPLY 0
# define RPL_ENDOFNAMES 0
# define RPL_INVITING 0
# define RPL_AWAY 0

// COMMANDS Messages

# define PASS_OK "\033[0;32mPassword correct !\033[0m Please set up Nickname now (use NICK command).\n"
# define NICK_OK "\033[0;32mNickname has been set !\033[0m Please set up Username now (use USER command).\n"
# define NICK_CHANGED "\033[0;32mYour nickname has been changed successfuly.\n\033[0m"
# define USERNAME_OK "\033[0;32mUsername has been set.\033[0m\n"
# define JOINED_CHANNEL "\033[0;32mYou join the channel\n\033[0m"
# define LEFT_CHANNEL " has left the channel.\n"

// CLIENT Error messages

# define ERR_CLIENT "\033[0;31mDefault Client Exception\n\033[0m"
# define ERR_CLIENT_BLOCK "\033[0;31mError: Could not set client I/O operations to non-blocking.\n\033[0m"


// Perror messages

# define PERR_SEND "send: "


// Constructor messages

// # define SERVER_C ""

#endif