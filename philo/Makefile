# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/16 13:55:41 by brmaria-          #+#    #+#              #
#    Updated: 2025/11/01 12:59:47 by brmaria-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# name
NAME = philo

# compiler and flags:
CC = cc
CFLAGS = -Wall -Wextra -Werror

# basic and additional files:
SRC =	0-main.c\
		
OBJ = $(SRC:.c=.o)

RM = rm -rf

# standard rule:
all: $(NAME)

# compile and create the lib:
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "Let's build this Philosophers! Done!"

# compile .o:
%.o: %.c push_swap.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiling $<..."
	
# clean files:
clean:
	@$(RM) $(OBJ)
	@echo "Off with these files!"

# clean everything, .o and libft.a:
fclean: clean
	@$(RM) $(NAME)
	@echo "Binary gone!"

# recompile all:
re: fclean all

.PHONY: all re clean fclean