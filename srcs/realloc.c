/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2019/12/09 21:24:35 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static void	*ft_reallocate_data(t_zone **zone, t_block *block, void *ptr, \
	size_t size)
{
	void  	*new_ptr;
	new_ptr = NULL;

	// this is the last block, just need to change the block size;
	if(block->next == NULL)
	{
		(*zone)->remaining += (int)((block->size_and_flag >> 1) - size);
		block->size_and_flag = (size << 1);
		new_ptr = ptr;
	}
	else
	{
		new_ptr = malloc(size);
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
};

static void	*ft_reallocate_large_data(void *ptr, size_t size)
{
	t_block	*block;
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = NULL;
	block = g_map.large;
	// aligned_size = ft_align_chunk(size, PAGE_SIZE);
	while(block && ((void *)block + BLOCK_SIZE != ptr))
		block = block->next;
	if(block)
	{
		if((block->size_and_flag >> 1) > size)
			copy_size = size;
		else
			copy_size = (block->size_and_flag >> 1);
		new_ptr = malloc(size);
		new_ptr = ft_memcpy(new_ptr, ptr, (block->size_and_flag >> 1));
		free(ptr);
	}
	return (new_ptr);
};

void		*realloc(void *ptr, size_t size)
{
	t_zone	**zone;
	t_block	*block;
	size_t	zone_size;
	size_t	aligned_size;

	if(ptr == NULL || (int)size < 0)
		return malloc(size);
	zone = ft_find_zone(&zone_size, ptr);
	if(zone)
	{
		aligned_size = ft_align_chunk(size, zone_size);
		block = (*zone)->block;
		while(block && ((void *)block + BLOCK_SIZE != ptr))
			block = block->next;
		if(block)
		{
			if((block->size_and_flag >> 1) == aligned_size)
				return (ptr);
			else
				return (ft_reallocate_data(zone, block, ptr, aligned_size));
		}
	}
	else
		return (ft_reallocate_large_data(ptr, size));
	return (NULL);
};