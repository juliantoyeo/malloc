/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/18 03:02:46 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static t_block	*ft_create_block(t_zone *zone, size_t zone_size, size_t size, \
	t_block *new_location)
{
	t_block		*block;

	if (((((void *)zone + zone_size) - (void *)new_location)) \
		< (long)(BLOCK_SIZE + size))
		return (NULL);
	block = new_location;
	block->size_and_flag = (size << 1);
	block->next = NULL;
	if (zone->block == NULL)
		zone->block = block;
	zone->remaining -= (size + BLOCK_SIZE);
	return (block);
}

static void		ft_create_zone(t_zone **zone, size_t zone_size)
{
	void		*page;

	page = mmap(0, zone_size, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (page == MAP_FAILED)
		return ;
	(*zone) = page;
	(*zone)->remaining = zone_size - sizeof(t_zone);
	(*zone)->block = NULL;
}

static void		ft_split_block(t_zone *zone, t_block *block, size_t zone_size, \
	size_t size)
{
	int			remaining;
	t_block		*new_block;
	void		*new_location;

	remaining = (block->size_and_flag >> 1) - size;
	if (remaining >= BLOCK_SIZE)
	{
		new_location = (void *)block + size + BLOCK_SIZE;
		new_block = ft_create_block(zone, zone_size, remaining \
			- BLOCK_SIZE, new_location);
		if (new_block != NULL)
		{
			block->size_and_flag = (size << 1);
			zone->remaining -= size;
			new_block->size_and_flag ^= 1;
			new_block->next = block->next;
			block->next = new_block;
			zone->remaining += (new_block->size_and_flag >> 1);
		}
		else
			block->size_and_flag ^= 1;
	}
	else
		block->size_and_flag ^= 1;
}

static void		ft_get_block(t_zone *zone, size_t zone_size, size_t size, \
	t_block **block)
{
	void		*new_location;
	t_block		*prev;

	prev = NULL;
	while ((*block) && (((*block)->size_and_flag & 1) == 0 || \
		((*block)->size_and_flag >> 1) < size))
	{
		prev = (*block);
		(*block) = (*block)->next;
	}
	if ((*block) == NULL)
	{
		if (zone->block == NULL)
			new_location = (void *)zone + sizeof(t_zone);
		else
		{
			new_location = (void *)prev + (prev->size_and_flag >> 1)
				+ BLOCK_SIZE;
		}
		(*block) = ft_create_block(zone, zone_size, size, new_location);
		if (prev)
			prev->next = (*block);
	}
	else
		ft_split_block(zone, (*block), zone_size, size);
}

void			*ft_alloc_data(t_zone **zone, size_t zone_size, size_t len)
{
	size_t		size;
	t_block		*block;

	if ((*zone) == NULL)
	{
		ft_create_zone(zone, zone_size);
		if ((*zone) == NULL)
		{
			ft_print_error(INSUFFICENT, 0);
			return (NULL);
		}
	}
	block = (*zone)->block;
	size = ft_align_chunk(len, zone_size);
	if ((*zone)->remaining < size)
		return (NULL);
	ft_get_block((*zone), zone_size, size, &block);
	if (block == NULL)
		return (NULL);
	return (block + 1);
}
