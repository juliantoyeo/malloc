/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 16:25:44 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/20 17:26:10 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_map g_map = {NULL, NULL, NULL};
pthread_mutex_t	g_lock = PTHREAD_MUTEX_INITIALIZER;

static void	*ft_alloc_data_large(t_block **l_block, size_t len)
{
	size_t	size;
	t_block	*new_block;
	t_block *last_block;

	last_block = (*l_block);
	size = ft_align_chunk(len + BLOCK_SIZE, PAGE_SIZE);
	new_block = mmap(0, size, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_block == MAP_FAILED)
	{
		ft_print_error(ERR_LARGE, len);
		return (NULL);
	}
	new_block->size_and_flag = ((size - BLOCK_SIZE) << 1);
	new_block->next = NULL;
	if ((*l_block) == NULL)
		(*l_block) = new_block;
	else
	{
		while (last_block->next)
			last_block = last_block->next;
		last_block->next = new_block;
	}
	return (new_block + 1);
}

void		*malloc(size_t size)
{
	void	*pointer;

	pointer = NULL;
	if (INT64_MAX < size && size <= UINT64_MAX)
		return (NULL);
	pthread_mutex_lock(&g_lock);
	if (size <= MAX_TINY_CHUNK_SIZE)
		pointer = ft_alloc_data(&g_map.tiny, TINY_ZONE_SIZE, size);
	else if (size <= MAX_SMALL_CHUNK_SIZE)
		pointer = ft_alloc_data(&g_map.small, SMALL_ZONE_SIZE, size);
	else
		pointer = ft_alloc_data_large(&g_map.large, size);
	pthread_mutex_unlock(&g_lock);
	return (pointer);
}
