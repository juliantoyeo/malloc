/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/02 21:07:12 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
// #include <stdio.h>

static void	ft_print_alloc_mem(t_block *block, size_t *total)
{
	void	*start;
	void	*end;
	size_t	size;

	while (block)
	{
		pthread_mutex_lock(&g_lock);
		start = (void *)block + BLOCK_SIZE;
		end = start + (block->size_and_flag >> 1);
		size = end - start;
		(*total) += size;
		pthread_mutex_unlock(&g_lock);
		ft_printf("%p - %p : %d bytes\n", start, end, size);
		ft_printf("im the block free : %d\n", block->size_and_flag & 1);
		block = block->next;
	}
}

void		show_alloc_mem(void)
{
	size_t	total;

	total = 0;
	// pthread_mutex_lock(&g_lock);
	ft_printf("TINY : %p\n", g_map.tiny);
	if (g_map.tiny)
		ft_print_alloc_mem(g_map.tiny->block, &total);
	ft_printf("SMALL : %p\n", g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small->block, &total);
	ft_printf("LARGE : %p\n", g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total);
	ft_printf("Total : %d bytes\n", total);
	// pthread_mutex_unlock(&g_lock);
}
