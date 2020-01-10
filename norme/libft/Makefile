# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/11 14:15:58 by jyeo              #+#    #+#              #
#    Updated: 2020/01/05 20:55:47 by juliantoyeo      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

FLAG = -Wall -Wextra -Werror

CC = gcc

LIBFT_DIR = 		./libft

LIBFT_SRC = 		libft/ft_atoi.c\
      				libft/ft_memmove.c\
      				libft/ft_putendl_fd.c\
      				libft/ft_bzero.c\
      				libft/ft_memset.c\
      				libft/ft_putnbr.c\
      				libft/ft_memcmp.c\
      				libft/ft_putchar.c\
      				libft/ft_putnbr_fd.c\
      				libft/ft_memcpy.c\
      				libft/ft_putchar_fd.c\
      				libft/ft_putstr.c\
      				libft/ft_memdel.c\
      				libft/ft_putendl.c\
      				libft/ft_putstr_fd.c\
					libft/ft_print_base.c\


INCLUDES = -I includes

LIBFT_OBJ = $(LIBFT_SRC:.c=.o)

ALL_OBJ = $(LIBFT_OBJ)

all: $(NAME)

$(NAME): $(ALL_OBJ)
	@ar rc $(NAME) $(ALL_OBJ)
	@ranlib $(NAME)

$(LIBFT_OBJ): %.o: %.c
	@$(CC) $(FLAG) $(INCLUDES) -c $^ -o $@

clean:
	@/bin/rm -f $(ALL_OBJ)

fclean:	clean
	@/bin/rm -f $(NAME)

re:	fclean all
