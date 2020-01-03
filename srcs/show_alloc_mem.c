/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/03 23:35:24 by juliantoyeo      ###   ########.fr       */
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
		start = (void *)block + BLOCK_SIZE;
		end = start + (block->size_and_flag >> 1);
		size = end - start;
		(*total) += size;
		ft_print_base((long)start, 16);
		ft_putstr(" - ");
		ft_print_base((long)end, 16);
		ft_putstr(" : ");
		ft_putnbr(size);
		ft_putstr(" bytes\n");
		ft_putstr("im the block free : ");
		ft_putnbr(block->size_and_flag & 1);
		ft_putstr("\n");
		// ft_printf("%p - %p : %d bytes\n", start, end, size);
		// ft_printf("im the block free : %d\n", block->size_and_flag & 1);
		block = block->next;
	}
}

void		ft_print_zone(char *str, long addr)
{
	ft_putstr(str);
	ft_putstr(" : ");
	ft_print_base(addr, 16);
	ft_putchar('\n');
}

void		show_alloc_mem(void)
{
	size_t	total;

	total = 0;
	pthread_mutex_lock(&g_lock);
	ft_print_zone("TINY", (long)g_map.tiny);
	if (g_map.tiny)
		ft_print_alloc_mem(g_map.tiny->block, &total);
	ft_print_zone("SMALL", (long)g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small->block, &total);
	ft_print_zone("LARGE", (long)g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total);
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr(" bytes\n");
	pthread_mutex_unlock(&g_lock);
}
