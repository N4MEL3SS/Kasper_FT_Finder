NAME = finder
CXX = gcc
CXXFLAGS = -Wall -Wextra -Werror
DEBUGFLAGS = -g -fsanitize=address
RM = rm -rf

#	SOURCE
SRCS_DIR = ./
SRCS = finder.c

#	OBJECT_FILES
OBJS_DIR = ./objs/
OBJS_DIR_NAME = objs
OBJS_LIST = $(patsubst %.c, %.o, $(SRCS))
OBJS = $(addprefix $(OBJS_DIR), $(OBJS_LIST))

#	COLORS
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
END = \033[0m

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)--- $(NAME) created. ---$(END)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR_NAME)
	@echo "$(GREEN)--- Object files directory was created. ---$(END)"

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c
	@$(CXX) $(CXXFLAGS) -c $< -o $@

debug: fclean $(OBJS_DIR) $(OBJS)
	$(CXX) $(DEBUGFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)--- $(NAME) created. ---$(END)"

clean:
	$(RM) $(OBJS)
	@echo "$(RED)--- Object files were deleted. ---$(END)"

fclean: clean
	$(RM) $(OBJS_DIR_NAME)
	$(RM) $(NAME)
	@echo "$(RED)--- $(NAME) was deleted. ---$(END)"

re: fclean all

.PHONY: all clean fclean re
