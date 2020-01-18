/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/18 02:33:13 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static void *ft_reallocate(t_block *block, void *ptr, size_t size)
{
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = NULL;
	if ((block->size_and_flag >> 1) > size)
		copy_size = size;
	else
		copy_size = (block->size_and_flag >> 1);
	pthread_mutex_unlock(&g_lock);
	new_ptr = malloc(size);
	if (new_ptr != NULL)
	{
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}

static void	*ft_reallocate_data(t_zone **zone, size_t zone_size, \
	void *ptr, size_t size)
{
	t_block	*block;
	void	*new_ptr;
	size_t	aligned_size;

	new_ptr = NULL;
	block = (*zone)->block;
	aligned_size = ft_align_chunk(size, zone_size);
	while (block && ((void *)block + BLOCK_SIZE != ptr))
		block = block->next;
	if (block)
	{
		if ((block->size_and_flag >> 1) == aligned_size)
		{
			pthread_mutex_unlock(&g_lock);
			return (ptr);
		}
		else
			new_ptr = ft_reallocate(block, ptr, size);
	}
	return (new_ptr);
}

static void	*ft_reallocate_large_data(void *ptr, size_t size)
{
	t_block	*block;
	void	*new_ptr;
	size_t	aligned_size;
	
	new_ptr = NULL;
	block = g_map.large;
	aligned_size = ft_align_chunk(size, PAGE_SIZE);
	while (block && ((void *)block + BLOCK_SIZE != ptr))
		block = block->next;
	if (block)
	{
		if ((block->size_and_flag >> 1) == (aligned_size - BLOCK_SIZE))
		{
			pthread_mutex_unlock(&g_lock);
			return (ptr);
		}
		else
			new_ptr = ft_reallocate(block, ptr, size);
	}
	else
	{
		pthread_mutex_unlock(&g_lock);
		return (malloc(size));
	}
	return (new_ptr);
}

void		*realloc(void *ptr, size_t size)
{
	t_zone	**zone;
	size_t	zone_size;
	
	// ft_putstr("realloc with size: ");
	// ft_putnbr(size);
	// ft_putchar(' ');
	// ft_print_base((long)ptr, 16);
	// ft_putchar('\n');
	if (ptr == NULL || (int)size < 0)
		return (malloc(size));
	pthread_mutex_lock(&g_lock);
	zone = ft_find_zone(&zone_size, ptr);
	if (zone)
		return (ft_reallocate_data(zone, zone_size, ptr, size));
	return (ft_reallocate_large_data(ptr, size));
}
