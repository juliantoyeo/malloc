# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyeo <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 14:15:58 by jyeo              #+#    #+#              #
#    Updated: 2019/12/09 20:00:51 by jyeo             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


MALLOC = malloc

FLAG = -Wall -Wextra -Werror

CC = gcc

SRC =     srcs/main.c\

INCLUDES = -I includes

OBJ = $(SRC:.c=.o)
LIBS = -L libft/ -lft
all: lib main

lib:
	@make -C libft/

main: $(OBJ)
	$(CC) $(FLAG) -o $(MALLOC) $(OBJ) $(LIBS)

$(OBJ): %.o: %.c
	$(CC) $(FLAG) $(INCLUDES) -c $< -o $@

clean:
	@/bin/rm -f $(OBJ)
	make clean -C libft/

fclean:	clean
	@/bin/rm -f $(MALLOC)
	make fclean -C libft/

re:	fclean all
