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

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

LINK = libft_malloc.so

MALLOC = malloc

FLAG = -Wall -Wextra -Werror

CC = gcc

SRC =     srcs/malloc.c\
					srcs/free.c\
					srcs/realloc.c\
					srcs/show_alloc_mem.c\
					srcs/alloc_data.c\
					srcs/tool.c\

INCLUDES = -I includes

OBJ = $(SRC:.c=.o)
LIBS = -L libft/ -lft
all: lib $(NAME)

lib:
	@make -C libft/

$(NAME): $(OBJ)
	$(CC) $(FLAG) $(OBJ) $(LIBS) -shared -o $(NAME)
	@/bin/rm -f $(LINK)
	ln -s $(NAME) $(LINK)

$(OBJ): %.o: %.c
	$(CC) $(FLAG) $(INCLUDES) -c $< -o $@

test: $(NAME)
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test0.c -o test0
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test1.c -o test1
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test2.c -o test2
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test3.c -o test3
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test3-1.c -o test3-1
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test4.c -o test4
	$(CC) $(INCLUDES) -w -L. -lft_malloc test/test5.c -o test5
	@echo test0 ===================================================
	@/usr/bin/time -l ./test0
	@echo test1 ===================================================
	@/usr/bin/time -l ./test1
	@echo test2 ===================================================
	@sh run.sh /usr/bin/time -l ./test2
	@echo test3 ===================================================
	@sh run.sh ./test3
	@echo test3-1 =================================================
	@sh run.sh ./test3-1
	@echo test4 ===================================================
	@sh run.sh ./test4
	@echo test5 ===================================================
	@./test5


clean:
	@/bin/rm -f $(OBJ)
	make clean -C libft/

fclean:	clean
	@/bin/rm -f $(NAME) $(LINK)
	make fclean -C libft/

re:	fclean all
