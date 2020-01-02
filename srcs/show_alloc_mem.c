/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/02 17:03:08 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdio.h>

static void	ft_print_alloc_mem(t_block *block, size_t *total)
{
	void	*start;
	void	*end;
	size_t	size;

	while (block)
	{
		start = (void *)block + BLOCK_SIZE;
		end = start + (block->size_and_flag >> 1);
		size = end - start;
		(*total) += size;
		printf("%p - %p : %zu bytes\n", start, end, size);
		// printf("im the block free : %d\n", block->size_and_flag & 1);
		block = block->next;
	}
}

void		show_alloc_mem(void)
{
	size_t	total;

	total = 0;
	printf("TINY : %p\n", g_map.tiny);
	if (g_map.tiny)
		ft_print_alloc_mem(g_map.tiny->block, &total);
	printf("SMALL : %p\n", g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small->block, &total);
	printf("LARGE : %p\n", g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total);
	printf("Total : %zu bytes\n", total);
}
