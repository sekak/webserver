RED = \033[0;31m
GREEN = \033[0;32m
REGULAR = \033[0;37m

Name = webserv
file = main.cpp  

OBJ_DIR = object
SRC_DIR = src
objects = $(addprefix object/, $(file:.cpp=.o))
srcs = $(addprefix src/, $(file))

RM = rm -rf 
CC = c++
CFLAG = -Wall -Wextra -Werror

$(Name): $(objects)
	@echo "$(RED) __/\__ "
	@echo " \    / "
	@echo " /_  _\ "
	@echo "   \/   $(REGULAR)" 
	@echo "$(GREEN)              _                         "
	@echo "__      _____| |__  ___  ___ _ ____   __"
	@echo "\ \ /\ / / _ \ '_ \/ __|/ _ \ '__\ \ / /"
	@echo " \ V  V /  __/ |_) \__ \  __/ |   \ V / "
	@echo "  \_/\_/ \___|_.__/|___/\___|_|    \_/ $(REGULAR)\n"
	@echo "$(RED)                                                                  __/\__ "
	@echo "								  \    / "
	@echo "								  /_  _\ "
	@echo "								    \/   $(REGULAR)" 
	@$(CC) $(CFLAG) -o $(Name) $(objects)
	@echo "$(GREEN)compiling success!$(REGULAR)"

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAG) -c $< -o $@


all : $(Name)
	

clean: 
	@$(RM) $(objects)

fclean: clean
	@$(RM) $(Name) $(OBJ_DIR)

re: fclean all

