#ifndef DEFINES_HPP
# define DEFINES_HPP


// Libraries

# include <map>
# include <ctime>
# include <vector>
# include <poll.h>
# include <cstdio>
# include <sstream>
# include <cstdlib>
# include <cstring>
# include <csignal>
# include <fcntl.h>
# include <unistd.h>
# include <limits.h>
# include <iostream>
# include <typeinfo>
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

# define RED_ITALIC "\x1b[31m \x1b[3m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define ORANGE "\033[0;33m"
# define BLUE "\033[0;36m"
# define PINK "\033[0;35m"
# define NC "\033[0m"
# define NC_ITALIC "\x1b[0m"


// External values

extern bool		doSignal;


// Values

# define ARG_NB 3
# define ERROR -1
# define MAX_CLIENTS 10
# define TIMEOUT 30000
# define BUFFERSIZE 1024
# define NOT_WHITESPACE 0
# define NO_SOCKET 0
# define EQUAL 0
# define EMPTY ""
# define SPACE " "
# define MAX_NICK_LEN 9
# define PATTERN_COUNT_CHAN 11
# define PATTERN_COUNT 8
# define EXPECTED_USER_PARAM 5
# define INCOMING_MSG "\033[0;36m<< \033[0m"
# define OUTGOING_MSG "\033[0;35m>> \033[0m"
# define CONNECTED true
# define DISCONNECTED false
# define UNKNOWN_TYPE 0
# define USER_TYPE 1
# define CHANNEL_TYPE 2
# define USER_INFO_LINES 5
# define CHAN_INFO_LINES 3
# define ADD_MODE 1
# define REMOVE_MODE -1
# define NO_MODE 0
# define GOOD_LIMIT 42
# define BAD_LIMIT -42
# define CLIENT_FOUND 24
# define CLIENT_NOT_FOUND -24


// Server specifications

# define HOST_NAME "localhost"
# define SERVER_NAME "irc"


# define NEWLINE '\n'
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
# define USER_NOT_ENTERED "\033[0;33m> Please set Username first (use USER command) <\033[0m"
# define PASS_ALREADY_ENTERED "\033[0;33m> Password has already been entered <\033[0m"
# define NOT_CONNECTED "\033[0;33m> You are not connected yet. Use PASS, NICK, USER <\033[0m"
# define USER_PARAM_KO "\033[0;33m> Please try again with correct parameters <\033[0m"

# define ERR_NOSUCHNICK			"\033[0;33m401 <client> <arg> :No such nick/channel\033[0m"
# define ERR_NOSUCHCHANNEL		"\033[0;33m403 <client> <channelName> :No such channel\033[0m"
# define ERR_CANNOTSENDTOCHAN	"\033[0;33m404 <client> <channelName> :Cannot send to channel\033[0m"
# define ERR_TOOMANYCHANNELS	"\033[0;33m405 <client> <channelName> :You have joined too many channels\033[0m"
# define ERR_NOORIGIN			"\033[0;33m409 <client> :No origin specified\033[0m"
# define ERR_NOTEXTTOSEND		"\033[0;33m412 <client> :No text to send\033[0m"
# define ERR_UNKNOWNCOMMAND		"\033[0;33m421 <client> <command> :Unknown command\033[0m"
# define ERR_NONICKNAMEGIVEN	"\033[0;33m431 <client> :No nickname given\033[0m"
# define ERR_ERRONEUSNICKNAME	"\033[0;33m432 <client> <arg> :Erroneus nickname\033[0m"
# define ERR_NICKNAMEINUSE		"\033[0;33m433 <client> <arg> :Nickname is already in use and has been adjusted\033[0m"
# define ERR_USERNOTINCHANNEL	"\033[0;33m441 <client> <arg> :They aren't on that channel\033[0m"
# define ERR_NOTONCHANNEL		"\033[0;33m442 <client> <channelName> :You're not on that channel\033[0m"
# define ERR_USERONCHANNEL		"\033[0;33m443 <client> <arg> :is already on channel\033[0m"
# define ERR_NEEDMOREPARAMS		"\033[0;33m461 <client> <command> :Not enough parameters\033[0m"
# define ERR_ALREADYREGISTERED	"\033[0;33m462 <client> :You may not reregister\033[0m"
# define ERR_PASSWDMISMATCH		"\033[0;33m464 :Password incorrect\033[0m"
# define ERR_CHANNELISFULL		"\033[0;33m471 <client> <channelName> :Cannot join channel (+l)\033[0m"
# define ERR_UNKNOWNMODE		"\033[0;33m472 <arg> :is unknown mode char to me\033[0m"
# define ERR_INVITEONLYCHAN		"\033[0;33m473 <client> <channelName> :Cannot join channel (+i)\033[0m"
# define ERR_BADCHANNELKEY		"\033[0;33m475 <client> <channelName> :Cannot join channel (+k)\033[0m"
# define ERR_BADCHANMASK		"\033[0;33m476 <channelName> :Bad Channel Mask\033[0m"
# define ERR_CHANOPRIVSNEEDED	"\033[0;33m482 <client> <channelName> :You're not channel operator\033[0m"
# define TOO_MANY_PARAM			"\033[0;33m:<client> <command> :Too many parameters\033[0m"


//Replies


# define RPL_WELCOME			"001 Welcome to the Internet Relay Network <client>!<user>@<host>"
# define RPL_YOURHOST			"002 Your host is <servername>, running version <ver>"
# define RPL_CREATED			"003 This server was created <date>"
# define RPL_MYINFO				"004 <servername> <version> <available user modes> <available channel modes>"
// # define RPL_AWAY				"301 <client> :<away message>"
# define RPL_CHANNELMODEIS		"324 <channelName> <arg>"
# define RPL_UNIQOPIS			"325 <channelName> <client>"
# define RPL_TOPIC				":<nickname> TOPIC <channelName> <topic>"
# define RPL_NOTOPIC			"331 <channelName> :Mo topic is set"
// # define RPL_INVITING			":<inviter> INVITE <invitee> <channelName>"
// # define RPL_INVITELIST			"346 <channelName> <invitemask>"
// # define RPL_ENDOFINVITELIST	"347 <channelName> :End of channel invite list"
// # define RPL_EXCEPTLIST			"348 <channelName> <exceptionmask>"
// # define RPL_ENDOFEXCEPTLIST	"349 <channelName> :End of channel exception list"
// # define RPL_ENDOFBANLIST		"368 <channelName> :End of channel ban list"
# define RPL_NAMREPLY 			"353 <client> = <channelName> :<nicknames>"
# define RPL_ENDOFNAMES			"366 <channelName> :End of NAMES list"
# define RPL_BANLIST			"367 <channelName> <banmask>"
# define RPL_QUIT				" has left the server.\n"
# define RPL_NICK				":<oldnickname>!~<username>@<clientIP> NICK :<nickname>"
// # define RPL_NICKWASSET			"NICK <nickname>"


// COMMANDS Messages

# define PASS_OK "\033[0;32mPassword correct !\033[0m Please set up Nickname now (use NICK command).\n"
# define NICK_OK "\033[0;32mNickname has been set !\033[0m Please set up Username now (use USER command).\n"
# define NICK_CHANGED "\033[0;32mYour nickname has been changed successfuly.\n\033[0m"
# define USERNAME_OK "\033[0;32mUsername has been set.\033[0m\n"
# define JOINED_CHANNEL "\033[0;32mYou join the channel\n\033[0m"
# define LEFT_CHANNEL " has left the channel.\n"
# define PONG "PONG <servername> :<client>"


// Signal

# define SIGNAL_EXIT "  exit"


// CLIENT Error messages

# define ERR_CLIENT "\033[0;31mDefault Client Exception\n\033[0m"
# define ERR_CLIENT_BLOCK "\033[0;31mError: Could not set client I/O operations to non-blocking.\n\033[0m"


// Perror messages

# define PERR_SEND "send: "



#endif