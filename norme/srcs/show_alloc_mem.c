/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/04 02:44:11 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static void	ft_print_block_status(t_block *block)
{
	if (block->size_and_flag & 1)
		ft_putstr(" \e[32m Free \e[0m");
	else
		ft_putstr(" \e[31m In Use \e[0m");
}

static void	ft_print_alloc_mem(t_block *block, size_t *total, int option)
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
		ft_putstr(" bytes");
		if (option == 1)
			ft_print_block_status(block);
		ft_putstr("\n");
		block = block->next;
	}
}

static void	ft_print_zone(char *str, long addr)
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
		ft_print_alloc_mem(g_map.tiny->block, &total, 0);
	ft_print_zone("SMALL", (long)g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small->block, &total, 0);
	ft_print_zone("LARGE", (long)g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total, 0);
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr(" bytes\n");
	pthread_mutex_unlock(&g_lock);
}

void		show_alloc_mem_free(void)
{
	size_t	total;

	total = 0;
	pthread_mutex_lock(&g_lock);
	ft_print_zone("TINY", (long)g_map.tiny);
	if (g_map.tiny)
		ft_print_alloc_mem(g_map.tiny->block, &total, 1);
	ft_print_zone("SMALL", (long)g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small->block, &total, 1);
	ft_print_zone("LARGE", (long)g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total, 1);
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr(" bytes\n");
	pthread_mutex_unlock(&g_lock);
}
