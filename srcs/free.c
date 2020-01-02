/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/02 21:13:15 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static void	ft_delete_block(t_zone **zone, size_t zone_size)
{
	t_block	*prev;
	t_block	*block;

	prev = NULL;
	block = (*zone)->block;
	while (block->next)
	{
		prev = block;
		block = block->next;
	}
	ft_bzero(block, (block->size_and_flag >> 1) + BLOCK_SIZE);
	block = NULL;
	(*zone)->remaining += BLOCK_SIZE;
	if (prev)
		prev->next = block;
	else
	{
		zone_size = 0;
		//if the program reach this point, means that the current block is the last free block in the zone, and the whole zone should be unmapped
		(*zone)->block = NULL;
		munmap((*zone), zone_size);
	}
}

static void	ft_merge_block(t_block **block, t_block **next)
{
	(*block)->size_and_flag += \
		((((*next)->size_and_flag >> 1) + BLOCK_SIZE) << 1);
	(*block)->next = (*next)->next;
	ft_bzero((*next), ((*next)->size_and_flag >> 1) + BLOCK_SIZE);
	(*next) = NULL;
}

static void	ft_defrag(t_zone **zone, size_t zone_size, \
	t_block *block, t_block *prev)
{
	t_block	*tmp;
	size_t	size_freed;

	size_freed = (block->size_and_flag >> 1);
	if (prev && (prev->size_and_flag & 1) == 1)
	{
		tmp = block;
		block = prev;
		ft_merge_block(&block, &tmp);
		size_freed += BLOCK_SIZE;
	}
	if (block->next && (block->next->size_and_flag & 1) == 1)
	{
		tmp = block->next;
		ft_merge_block(&block, &tmp);
		size_freed += BLOCK_SIZE;
	}
	(*zone)->remaining += size_freed;
	if (block->next == NULL)
		ft_delete_block(zone, zone_size);
}

static int	ft_free_block(t_zone **zone, size_t zone_size, void *ptr)
{
	t_block	*prev;
	t_block	*block;

	if (zone == NULL)
		return (0);
	prev = NULL;
	block = (*zone)->block;
	while (block && ((void *)block + BLOCK_SIZE != ptr))
	{
		prev = block;
		block = block->next;
	}
	if (block)
	{
		block->size_and_flag = block->size_and_flag ^ 1;
		ft_defrag(zone, zone_size, block, prev);
	}
	else
		return (0);
	return (1);
}

void		free(void *ptr)
{
	size_t	zone_size;
	t_block	*prev;
	t_block	*block;

	prev = NULL;
	block = g_map.large;
	pthread_mutex_lock(&g_lock);
	if (ft_free_block(ft_find_zone(&zone_size, ptr), zone_size, ptr) == 0)
	{
		while (block && ((void *)block + BLOCK_SIZE != ptr))
		{
			prev = block;
			block = block->next;
		}
		if (block)
		{
			if (block == g_map.large)
				g_map.large = block->next;
			else if (prev)
				prev->next = block->next;
			munmap(block, ((block->size_and_flag >> 1) + BLOCK_SIZE));
		}
		// else
		// 	ft_printf("im invalid block %p\n", ptr);
	}
	pthread_mutex_unlock(&g_lock);
}
