/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:20:19 by jyeo              #+#    #+#             */
/*   Updated: 2019/12/09 21:21:09 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <sys/mman.h>
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# define BLOCK_SIZE 16
# define TINY_ZONE_SIZE (4096 * 8)
# define TINY_CHUNK_SIZE 16
# define MAX_TINY_CHUNK_SIZE 496
# define SMALL_ZONE_SIZE (4096 * 32)
# define SMALL_CHUNK_SIZE 512
# define MAX_SMALL_CHUNK_SIZE 4080

typedef	struct		s_block
{
	size_t			size_and_flag;
	struct s_block	*next;
}					t_block;

typedef	struct		s_zone
{
	size_t			remaining;
	t_block			*block;
}					t_zone;

typedef struct		s_map
{
	t_zone			*tiny;
	t_zone			*small;
	t_zone			*large;
}					t_map;

extern t_map		g_map;

// typedef	struct		s_block
// {
// 	size_t				  size;
//   // int             free;
// 	struct s_block	*next;
// }					        t_block;

// typedef struct s_block *t_block;

// struct s_block {
//   size_t size;
//   t_block next;
//   int free;
// }; 

// typedef	struct		s_zone
// {
// 	int				      fd;
// 	char			      *n_line;
// 	struct s_zone	  *next;
// }					        t_zone;

#endif