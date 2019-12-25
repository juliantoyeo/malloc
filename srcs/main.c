/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2019/12/09 21:24:35 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_map g_map = { NULL, NULL, NULL};

void	ft_print_alloc_mem(t_zone *zone, size_t *total)
{
	void		*start;
	void		*end;
	size_t		size;
	t_block		*block;

	block = zone->block;
	while(block)
	{
		start = (void *)block + BLOCK_SIZE;
		end = start + (block->size_and_flag >> 1);
		size = end - start;
		(*total) += size;
		ft_printf("%p - %p : %d bytes\n", start, end, size);
		ft_printf("im the block free : %d\n", block->size_and_flag & 1);
		block = block->next;
	}
};

void	show_alloc_mem()
{
	size_t total;

	total = 0;
	ft_printf("TINY : %p\n", g_map.tiny);
	if (g_map.tiny)
		ft_print_alloc_mem(g_map.tiny, &total);
	ft_printf("SMALL : %p\n", g_map.small);
	if (g_map.small)
		ft_print_alloc_mem(g_map.small, &total);
	ft_printf("LARGE : %p\n", g_map.large);
	if (g_map.large)
		ft_print_alloc_mem(g_map.large, &total);
	ft_printf("Total : %d bytes\n", total);
};

// Align the requested size to either 16bytes or 512bytes depends on the zone
size_t   ft_align_chunk(size_t len, size_t zone_size)
{
	if (zone_size == TINY_ZONE_SIZE)
		return (((((len)-1)>>4)<<4)+16);
	else
		return (((((len)-1)>>9)<<9)+512);
};

// Create a new block
t_block  *ft_create_block(t_zone *zone, size_t zone_size, size_t size, \
  t_block *prev_block)
{
	void    *new_location;
	t_block *block;

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
	return NULL; // not enough size to be given at the new location
	block = new_location;
	block->size_and_flag = (size << 1);
	block->next = NULL;
	if (zone->block == NULL)
		zone->block = block;
	return (block);
};

void    ft_create_zone(t_zone **zone, size_t zone_size)
{
	void    *page;

	page = mmap(0, zone_size, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0);
	(*zone) = page;
	(*zone)->remaining = zone_size - sizeof(t_zone);
	(*zone)->block = NULL;
};

void		ft_split_block(t_zone *zone, t_block *block, size_t zone_size, \
			size_t size)
{
	int		min_size;
	int		remaining;
	t_block	*new_block;

	if (zone_size == TINY_ZONE_SIZE)
		min_size = BLOCK_SIZE + TINY_CHUNK_SIZE;
	else
		min_size = BLOCK_SIZE + SMALL_CHUNK_SIZE;
	remaining = (block->size_and_flag >> 1) - size;
	// After the remaining size is calculated, assign the new allocated size to the block
	block->size_and_flag = (size << 1);
	// If the remaining size is enough to store even a metadata block, we split it and set the new block as free
	if (remaining >= BLOCK_SIZE)
	{
		new_block = ft_create_block(zone, zone_size, remaining - BLOCK_SIZE, block);
		new_block->size_and_flag ^= 1;
		new_block->next = block->next;
		block->next = new_block;
	}
};

void	ft_get_block(t_zone *zone, size_t zone_size, size_t size, \
	t_block **block)
{
	t_block *prev;

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
	if ((*zone)->remaining < (size + BLOCK_SIZE))
    	return (NULL); // not enough size to be given
	ft_get_block((*zone), zone_size, size, &block);
	if(block == NULL)
		return (NULL);
	//TODO redo the remaining calculation, if reuse the previous remaining
	(*zone)->remaining -= (size + BLOCK_SIZE);
	// ft_printf("remaining : %d\n", (*zone)->remaining);
	return (block + 1);
};

void    *ft_malloc(size_t len)
{
	void  *pointer;
	pointer = NULL;
	if(len <= 0)
		return (NULL);
	if(len <= MAX_TINY_CHUNK_SIZE)
	{
		pointer = ft_alloc_data(&g_map.tiny, TINY_ZONE_SIZE, len);
	}
	else if (len <= MAX_SMALL_CHUNK_SIZE)
	{
		pointer = ft_alloc_data(&g_map.small, SMALL_ZONE_SIZE, len);
	}
	return (pointer);
};

void	ft_free_block(t_zone **zone, void *ptr)
{
	t_block	*block;

	block = (*zone)->block;
	while(block && ((void *)block + BLOCK_SIZE != ptr))
	{
		block = block->next;
	}
	block->size_and_flag = block->size_and_flag ^ 1;
};

int		ft_find_block(void *ptr)
{
	void	*tiny_start;
	void	*tiny_end;
	void	*small_start;
	void	*small_end;

	tiny_start = g_map.tiny;
	tiny_end = tiny_start + TINY_ZONE_SIZE - 1;
	small_start = g_map.small;
	small_end = small_start + SMALL_ZONE_SIZE - 1;
	if (tiny_start < ptr && ptr < tiny_end)
	{
		ft_free_block(&g_map.tiny, ptr);
	}
	else if (small_start < ptr && ptr < small_end)
	{
		ft_free_block(&g_map.small, ptr);
	}
	return 0;
};

void 	ft_free(void *ptr)
{
	if(ft_find_block(ptr))
	{
		ft_printf("im valid block %p\n", ptr);
	}
	else
	{
		ft_printf("im invalid block %p\n", ptr);
	};
};

int main(int ac, char **av)
{
	ac = 0;
	av = NULL;
	// ft_malloc(ft_atoi(av[1]));

	// ft_malloc(4033);
	// ft_malloc(1);

	char *p1 = ft_malloc(1);
	// ft_free(p1);
	// char *s1 = ft_malloc(1024);
	// ft_free(s1);
	char *p2 = ft_malloc(17);
	// ft_free(p1);
	// char *s2 = ft_malloc(1024);
	// ft_free(s1);
	// ft_strcpy(p1, "0123456789abcdef~!!");
	char *p3 = ft_malloc(10);
	char *p4 = ft_malloc(10);
	// p1 = ft_malloc(1);
	// char *s3 = ft_malloc(512);
	ft_printf("p1 address : %p\n", p1);
	// ft_printf("s1 address : %p\n", s1);
	// ft_printf("p1 content : %s\n", p1);
	ft_printf("p2 address : %p\n", p2);
	// ft_printf("s2 address : %p\n", s2);
	ft_printf("p3 address : %p\n", p3);
	ft_printf("p4 address : %p\n", p4);
	// ft_printf("s3 address : %p\n", s3);
	// ft_free(s1);
	// ft_free(s2);
	show_alloc_mem();
	
	// char *p1 = malloc(512);
	// char *p2 = malloc(16);
	// char *p3 = malloc(17);
	// char *p4 = malloc(32);
	// ft_printf("p1 address : %p\n", p1);
	// ft_printf("p2 address : %p\n", p2);
	// ft_printf("p3 address : %p\n", p3);
	// ft_printf("p4 address : %p\n", p4);
	// void *ptr;
	// int i = 0;
	// while(i < 10)
	// {
	// 	ptr = malloc(100);
	// 	ft_printf("ptr address on ptr nbr %d : %p\n", i+1, ptr);
	// 	i++;
	// };

	// ft_printf("%d", ft_align16(ft_atoi(av[1])));
	// char *p1 = malloc(1);
	// t_block *b1 = malloc(sizeof(t_block));
	// int *num = malloc(sizeof(int));
	// ft_printf("%p\n", b1);
	// ft_printf("%p\n", b1 + 1);
	// ft_printf("%p\n", num);
	// ft_printf("%p\n", num + 1);
	// ft_printf("sizeof(int) : %d\n", sizeof(int));
	// char *p2 = malloc(10);
	// char *p3 = malloc(1024+1);
	// t_block *block;
	// t_block *block2;

	// char *map1 = mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	// char *map2 = mmap(NULL, 4097, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	// char *map3 = mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  // size_t size = 10;
  // id_t   flag = 1;
  // size_t size_and_flag = (size << 1) | flag;
  // ft_printf("%b\n",size);
  // ft_printf("%b\n",flag);
  // ft_printf("%b\n",size|flag);
  // ft_printf("%b\n",size_and_flag);
  // size_and_flag ^= 1;
  // ft_printf("%b\n",size_and_flag);
  // size_and_flag |= 1;
  // ft_printf("%b\n",size_and_flag);
  // ft_printf("size is %d\n",size_and_flag >> 1);
  
  
  // ft_printf("address diff between p1 and p2: %d\n", p2-p1);
  // ft_printf("address diff between p2 and p3: %d\n", p3-p2);
  // ft_strcpy(map2, "abcde");
  // munmap(map2, 10);
	return (0);
};
