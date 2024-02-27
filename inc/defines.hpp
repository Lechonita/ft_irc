#ifndef DEFINES_HPP
#define DEFINES_HPP

#ifdef DEBUG
#define debug(msg) std::cerr << "DEBUG: " << msg << std::endl
#else
#define debug(msg)
#endif

#include <iostream>

// Colors

#define RED "\033[0;31m"
#define GREEN "\033[1;32m"
#define NC "\033[0m"

// Values

#define ARG_NB 3
#define ERROR -1
#define MAX_CLIENTS 10
#define TIMEOUT 10000
#define CONNECTED true
#define DISCONNECTED false

// Error messages

#endif
