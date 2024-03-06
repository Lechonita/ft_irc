##################################### VARIABLES #####################################

PROJECT_NAME	= ircserv
NAME			= ./$(PROJECT_NAME)
CC				= c++
FLAGS			= -Wall -Wextra -Werror -std=c++98

##################################### INCLUDES #####################################

INC_DIR		= ./inc
INCLUDES	+= -I $(INC_DIR)

##################################### HEADERS #####################################

HDR			+= Server.hpp
HDR			+= Client.hpp
HDR			+= defines.hpp
HDR			+= Channel.hpp
HDR			+= Commands.hpp
# HDR			+= irc_utils.hpp

vpath %.h $(INC_DIR)

##################################### SOURCES #####################################

SRC_DIR		= ./src

SRC			+= main.cpp
SRC			+= Server.cpp
SRC			+= Client.cpp
SRC			+= Channel.cpp
SRC			+= Commands.cpp
SRC			+= NICK_utils.cpp
SRC			+= PASS_utils.cpp
# SRC			+= irc_utils.cpp
SRC			+= Client_disconnect.cpp
SRC			+= Client_message_reception.cpp


vpath %.cpp $(SRC_DIR)

##################################### OBJECTS #####################################

OBJ_DIR		= ./obj
OBJ			= $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))


##################################### TESTER ######################################

### Tester

TESTER_FOLDER	= tests/
TESTER			= $(TESTER_FOLDER)/tester.sh


ifeq ($(debug), true)
	CFLAGS	+= -fsanitize=address,undefined -g3
endif


##################################### BUILD #####################################

DEPS_DIR = $(OBJ_DIR)/.deps
DEPS = $(SRC:%.cpp=$(DEPS_DIR)/%.d)
DFLAGS = -MT $@ -MMD -MP

##################################### RULES #####################################

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
	@echo $(GREEN)"\n\n	⤳ Created $(NAME) ✨\n"$(DEF_COLOR)

-include $(DEPS)

$(OBJ) : $(OBJ_DIR)/%.o: %.cpp | dir
	@$(CC) $(FLAGS) $(DFLAGS) $(INCLUDES) -c $< -o $@
	@mv $(basename $@).d $(DEPS_DIR)/$(notdir $(basename $@)).d && touch $@
	@$(call PROGRESS_BAR, $(basename $(notdir $<)))

dir: $(OBJ_DIR)

test: all
	echo -e $(BLUE) "\n====> IRC TESTS"$(NC)"\n"
	$(TESTER)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)
	@mkdir $(DEPS_DIR)

clean:
	@echo $(ORANGE)"\nRemoving .o object files...\n"$(END)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(ORANGE)"Removing executable...\n"$(END)
	@rm -f $(NAME)
	@echo $(BLUE)"	⤳ All cleaned 🌊\n"$(END)

re: fclean all

.PHONY: all clean fclean re



##################################### DISPLAY #####################################

RED='\033[0;31m'
GREEN='\033[1;32m'
ORANGE='\033[0;33m'
YELLOW='\033[1;33m'
BLUE='\033[1;36m'
NC='\033[0m' # No Color

NB_OBJ = ${words $(SRC)}
COUNTER = 1
PROGRESS = 0
DONE = 100
SPACE = 0
FILL = 0
EMPTY = 0

define PROGRESS_BAR
	$(eval PROGRESS=$(shell echo $$(($(COUNTER) * 100 / $(NB_OBJ)))))
	$(eval DONE=$(shell echo $$(($(PROGRESS) * 30 / 100))))
	$(eval SPACE=$(shell echo $$((30 - $(DONE)))))
	$(eval FILL=$(shell printf "%*s" ${DONE} | sed 's/ /▶/g'))
	$(eval EMPTY=$(shell printf "%*s" ${SPACE} | sed 's/ /◀/g'))
	printf ${CLEAR}$(BLUE)"\rCompilation : "
	printf $(LGREEN)${FILL}$(LRED)${EMPTY}
	printf $(BLUE)" %3d%% %s.c" $(END) ${PROGRESS} $1
	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
endef
