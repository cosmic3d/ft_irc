# -=-=-=-=-	NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
NAME			= ircserv
MK			= Makefile

# -=-=-=-=-	CLRS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

RESET		:= \033[0;39m
BLACK		:= \033[0;30m
RED			:= \033[0;91m
GREEN		:= \033[0;92m
YELLOW		:= \033[0;93m
BLUE		:= \033[0;94m
MAGENTA		:= \033[0;95m
CYAN		:= \033[0;96m
GRAY		:= \033[0;90m
WHITE		:= \033[0;97m

# -=-=-=-=-	DIRS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
SRC_DIR	= src/
OBJ_DIR	= obj/
INC_DIR	= hdrs/

# -=-=-=-=-	CMNDS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
CC			= c++
SANS		= -fsanitize=address -g
CFLAGS		= -Wall -Werror -Wextra
CPPSTANDARD	= -std=c++98
RM			= rm -f
MKDIR		= mkdir -p
CP			= cp -f
MAKE		= make -s
MUTE		= &> /dev/null
# -=-=-=-=-	HEADERS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
HDRS		+= $(shell find $(INC_DIR) -name "*.hpp")
# -=-=-=-=-	SOURCES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRCS		:= $(shell find src -name "*.cpp")

# -=-=-=-=-	OBJECTS/DEPENDENCIES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

#OBJS		:= $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))#@cd $(RDL_DIR) $(MUTE) && ./configure $(MUTE)
OBJS		:= $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRCS))
DEPS		+= $(addsuffix .d, $(basename $(OBJS)))

# -=-=-=-=-	COMPILING -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
INCLUDE 	= -I$(INC_DIR)

all: $(NAME)


$(NAME):: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPSTANDARD) $(SANS) $(INCLUDE) $(OBJS) -o $(NAME)
	@echo "$(GREEN)🤖🖤FT_IRC COMPILED🖤🤖$(RESET)"

$(NAME)::
	@echo "$(BLUE)Nothing to be done for $@$(RESET)";

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(MK) $(HDRS)
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPSTANDARD) $(SANS) $(INCLUDE) -MMD -c $< -o $@
	@echo "$(YELLOW)> $@ COMPILED$(RESET)"

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "$(CYAN)Dependencies and objects removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME) succesfully removed$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY:	all clean fclean re