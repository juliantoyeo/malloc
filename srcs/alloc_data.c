/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  alloc_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2019/12/09 21:24:35 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

// Create a new block
static t_block	*ft_create_block(t_zone *zone, size_t zone_size, size_t size, \
  t_block *prev_block)
{
	void    	*new_location;
	t_block 	*block;

	// If this is the 1st block, the new location is base on the start of the zone + the size of zone struct 
	if (zone->block == NULL)
		new_location = (void *)zone + sizeof(t_zone);
	else
	// Else the new location is base on previous block location + the previous size + metadata block size
	{
		new_location = (void *)prev_block + (prev_block->size_and_flag >> 1)
			+ BLOCK_SIZE;
	}
	if (((((void *)zone + zone_size) - (void *)new_location)) \
		< (long)(BLOCK_SIZE + size))
	return (NULL); // not enough size to be given at the new location
	block = new_location;
	block->size_and_flag = (size << 1);
	block->next = NULL;
	if (zone->block == NULL)
		zone->block = block;
	zone->remaining -= (size + BLOCK_SIZE);
	return (block);
};

static void    	ft_create_zone(t_zone **zone, size_t zone_size)
{
	void    	*page;

	page = mmap(0, zone_size, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0);
	(*zone) = page;
	(*zone)->remaining = zone_size - sizeof(t_zone);
	(*zone)->block = NULL;
};

static void		ft_split_block(t_zone *zone, t_block *block, size_t zone_size, \
	size_t size)
{
	int			min_size;
	int			remaining;
	t_block		*new_block;

	if (zone_size == TINY_ZONE_SIZE)
		min_size = BLOCK_SIZE + TINY_CHUNK_SIZE;
	else
		min_size = BLOCK_SIZE + SMALL_CHUNK_SIZE;
	remaining = (block->size_and_flag >> 1) - size;
	// After the remaining size is calculated, assign the new allocated size to the block
	block->size_and_flag = (size << 1);
	zone->remaining -= size;
	// If the remaining size is enough to store even a metadata block, we split it and set the new block as free
	if (remaining >= BLOCK_SIZE)
	{
		new_block = ft_create_block(zone, zone_size, remaining - BLOCK_SIZE, block);
		new_block->size_and_flag ^= 1;
		new_block->next = block->next;
		block->next = new_block;
		zone->remaining += (new_block->size_and_flag >> 1);
	}
};

static void		ft_get_block(t_zone *zone, size_t zone_size, size_t size, \
	t_block **block)
{
	t_block 	*prev;

	prev = NULL;
	// iterate to find a free block, if it is free, check if the size is correct, if not iterate till the end of the chain
	while ((*block) && \
		(((*block)->size_and_flag & 1) == 0 || ((*block)->size_and_flag >> 1) < size))
	{
		prev = (*block);
		(*block) = (*block)->next;
	};
	// No free block to be use, create a new block and add it to the end of the chain list
	if((*block) == NULL)
	{
		(*block) = ft_create_block(zone, zone_size, size, prev);
		if (prev)
			prev->next = (*block);
	}
	// Found a free block, assign the new size and check if the remaining size is splitable 
	else
	{
		ft_split_block(zone, (*block), zone_size, size);
	}
};

void		*ft_alloc_data(t_zone **zone, size_t zone_size, size_t len)
{
	size_t  size;
	t_block *block;

	if ((*zone) == NULL)
		ft_create_zone(zone, zone_size);
	block = (*zone)->block;
	size = ft_align_chunk(len, zone_size);
	if ((*zone)->remaining < size)
    	return (NULL); // not enough size to be given
	ft_get_block((*zone), zone_size, size, &block);
	if(block == NULL)
		return (NULL);
	return (block + 1);
};
