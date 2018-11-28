# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/25 18:27:37 by mpetruno          #+#    #+#              #
#    Updated: 2018/11/09 15:43:40 by mpetruno         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

FLAGS = -Wall -Werror -Wextra

SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./includes/
LIB_DIR = ./libft/
BUILTIN_DIR = builtin/
LIB_INC_DIR = $(LIB_DIR)includes/
LIB = $(LIB_DIR)libft.a

MAIN_FILES = main.c \
			execute.c \
			signal.c \
			input.c \
			environ.c \
			environ_util.c \
			err.c

BUILTIN = builtin.c \
			exit.c \
			pwd.c \
			cd.c \
			env.c \
			setenv.c \
			unsetenv.c \
			echo.c

SRC_LIST = $(MAIN_FILES) \
			$(addprefix $(BUILTIN_DIR), $(BUILTIN))

OBJ_LIST = $(addprefix $(OBJ_DIR), $(SRC_LIST:.c=.o))

all: $(NAME)

$(NAME): $(LIB) $(OBJ_LIST)
	@$(CC) $(OBJ_LIST) $(LIB) -o $(NAME)
	@echo "$(NAME) - Done."

$(LIB):
	@make -C $(LIB_DIR) all --silent

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)
	@$(CC) $(FLAGS) -I $(INC_DIR) -I $(LIB_INC_DIR) -c $^ -o $@

clean:
	@echo "$(NAME): Cleaning object files... "
	@rm -rf $(OBJ_DIR)
	@make -C $(LIB_DIR) clean --silent

fclean: clean
	@echo "$(NAME): Removing $(NAME)... "
	@rm -f $(NAME)
	@make -C $(LIB_DIR) fclean --silent

re: fclean all

.PHONY = all clean fclean re