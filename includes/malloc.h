/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:20:19 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/05 20:39:20 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <sys/mman.h>
# include <pthread.h>
# include "../libft/includes/libft.h"
# define BLOCK_SIZE 16
# define TINY_ZONE_SIZE 1048576
# define TINY_CHUNK_SIZE 16
# define MAX_TINY_CHUNK_SIZE 496
# define SMALL_ZONE_SIZE 8388608
# define SMALL_CHUNK_SIZE 512
# define MAX_SMALL_CHUNK_SIZE 4096
# define PAGE_SIZE 4096
# define ERR_LARGE "ERR_LARGE"
# define INSUFFICENT "INSUFFICENT"

typedef	struct			s_block
{
	size_t				size_and_flag;
	struct s_block		*next;
}						t_block;

typedef	struct			s_zone
{
	size_t				remaining;
	t_block				*block;
}						t_zone;

typedef struct			s_map
{
	t_zone				*tiny;
	t_zone				*small;
	t_block				*large;
}						t_map;

extern t_map			g_map;
extern pthread_mutex_t	g_lock;

size_t					ft_align_chunk(size_t len, size_t zone_size);
t_zone					**ft_find_zone(size_t *size, void *ptr);
void					*ft_alloc_data(t_zone **zone, size_t zone_size, size_t len);
void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);
void					show_alloc_mem_free(void);
void					ft_print_error(char *err, size_t size);

#endif
