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

t_map g_map = {NULL, NULL, NULL};

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
	ft_printf("remaining : %d\n", zone->remaining);
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
	{
		if(len == 0)
			return (TINY_CHUNK_SIZE);
		else
			return ((((len-1)>>4)<<4)+TINY_CHUNK_SIZE);
	}
	else
		return ((((len-1)>>9)<<9)+SMALL_CHUNK_SIZE);
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
	return (NULL); // not enough size to be given at the new location
	block = new_location;
	block->size_and_flag = (size << 1);
	block->next = NULL;
	if (zone->block == NULL)
		zone->block = block;
	zone->remaining -= (size + BLOCK_SIZE);
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
	if ((*zone)->remaining < size)
    	return (NULL); // not enough size to be given
	ft_get_block((*zone), zone_size, size, &block);
	if(block == NULL)
		return (NULL);
	ft_printf("remaining : %d\n", (*zone)->remaining);
	return (block + 1);
};

void    *ft_malloc(size_t size)
{
	void  *pointer;
	pointer = NULL;
	if((int)size < 0)
		return (NULL);
	if(size <= MAX_TINY_CHUNK_SIZE)
	{
		pointer = ft_alloc_data(&g_map.tiny, TINY_ZONE_SIZE, size);
	}
	else if (size <= MAX_SMALL_CHUNK_SIZE)
	{
		pointer = ft_alloc_data(&g_map.small, SMALL_ZONE_SIZE, size);
	}
	return (pointer);
};

void	ft_delete_block(t_zone **zone, size_t zone_size)
{
	t_block	*prev;
	t_block	*block;

	prev = NULL;
	block = (*zone)->block;
	while(block->next)
	{
		prev = block;
		block = block->next;
	}
	ft_bzero(block, (block->size_and_flag >> 1) + BLOCK_SIZE);
	block = NULL;
	(*zone)->remaining += BLOCK_SIZE;
	if(prev)
		prev->next = block;
	else
	{
		//if the program reach this point, means that the current block is the last free block in the zone, and the whole zone should be unmapped
		munmap((*zone), zone_size);
		(*zone) = NULL;
	}
};

void	ft_merge_block(t_block **block, t_block **next)
{
	(*block)->size_and_flag += ((((*next)->size_and_flag >> 1) + BLOCK_SIZE) << 1);
	(*block)->next = (*next)->next;
	ft_bzero((*next), ((*next)->size_and_flag >> 1) + BLOCK_SIZE);
	(*next) = NULL;
};

void	ft_defrag(t_zone **zone, size_t zone_size, t_block *block, t_block *prev)
{
	t_block	*tmp;
	size_t	size_freed;

	size_freed = (block->size_and_flag >> 1);
	if(prev && (prev->size_and_flag & 1) == 1)
	{
		ft_printf("prev block is free\n");
		tmp = block;
		block = prev;
		ft_merge_block(&block, &tmp);
		size_freed += BLOCK_SIZE;
	}
	if(block->next && (block->next->size_and_flag & 1) == 1)
	{
		ft_printf("next block is free\n");
		tmp = block->next;
		ft_merge_block(&block, &tmp);
		size_freed += BLOCK_SIZE;
	}
	(*zone)->remaining += size_freed;
	if(block->next == NULL)
	{
		ft_printf("im the last block that is free %p\n", block);
		ft_delete_block(zone, zone_size);
	}
};

int		ft_free_block(t_zone **zone, size_t zone_size, void *ptr)
{
	t_block	*prev;
	t_block	*block;

	if(zone == NULL)
		return (0);
	// ft_printf("im size : %d\n", zone_size);
	prev = NULL;
	block = (*zone)->block;
	while(block && ((void *)block + BLOCK_SIZE != ptr))
	{
		prev = block;
		block = block->next;
	}
	block->size_and_flag = block->size_and_flag ^ 1;
	// (*zone)->remaining += (block->size_and_flag >> 1);
	ft_defrag(zone, zone_size, block, prev);
	return (1);
};

t_zone		**ft_find_zone(size_t *size, void *ptr)
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
		(*size) = TINY_ZONE_SIZE;
		return (&g_map.tiny);
		// ft_free_block(&g_map.tiny, TINY_ZONE_SIZE, ptr);
	}
	else if (small_start < ptr && ptr < small_end)
	{
		(*size) = SMALL_ZONE_SIZE;
		return (&g_map.small);
		// ft_free_block(&g_map.small, SMALL_ZONE_SIZE, ptr);
	}
	return (NULL);
};

void 	ft_free(void *ptr)
{
	size_t	zone_size;

	if(ft_free_block(ft_find_zone(&zone_size, ptr), zone_size, ptr))
	{
		ft_printf("im valid block %p\n", ptr);
	}
	else
	{
		ft_printf("im invalid block %p\n", ptr);
	}
};

void	*ft_reallocate_data(t_zone **zone, t_block *block, void *ptr, size_t size)
{
	void  *new_ptr;
	new_ptr = NULL;

	if(block->next == NULL)
	{
		(*zone)->remaining += (int)((block->size_and_flag >> 1) - size);
		block->size_and_flag = (size << 1);
		new_ptr = ptr;
	}
	else
	{
		new_ptr = ft_malloc(size);
		ft_memcpy(new_ptr, ptr, size);
		ft_printf("im size : %d\n", size);
		ft_printf("ptr content : %s\n", ptr);
		ft_printf("new_ptr content : %s\n", new_ptr);
		ft_free(ptr);
	}
	return (new_ptr);
};

void	*ft_realloc(void *ptr, size_t size)
{
	t_zone	**zone;
	t_block	*block;
	size_t	zone_size;
	size_t	aligned_size;

	if(ptr == NULL || (int)size < 0)
		return ft_malloc(size);
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
	return (NULL);
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
	char *p3 = ft_malloc(0);
	// show_alloc_mem();
	// ft_free(p1);
	// show_alloc_mem();
	// ft_free(p2);
	// show_alloc_mem();
	char *p4 = ft_malloc(16);
	show_alloc_mem();
	ft_strcpy(p4, "0123456789abcdef~!!");
	// char *str = malloc(1);
	// ft_printf("str address : %p\n", str);
	char  *p5 = ft_realloc(p4, 32);
	// show_alloc_mem();
	ft_printf("p5 address : %p\n", p5);
	ft_printf("p5 content : %s\n", p5);
	ft_printf("p4 content : %s\n", p4);
	// char *str = malloc(1);
	// ft_free(str);
	// ft_free(p3);
	// ft_free(s1);
	// ft_free(s2);
	// show_alloc_mem();
	// ft_free(p2);
	// show_alloc_mem();
	// ft_free(p4);
	// show_alloc_mem();
	// ft_free(p1);
	// show_alloc_mem();
	// p1 = ft_malloc(1);
	// ft_free(p4);
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
	// show_alloc_mem();
	// ft_free(p1);
	// show_alloc_mem();
	// ft_free(p2);
	show_alloc_mem();
	
	// char *str = ft_malloc(1);
	// char *str1 = malloc(17);
	// char *str2;
	// ft_strcpy(str1, "0123456789abcdef0");
	// ft_putstr(str1);
	// ft_putchar('\n');
	// str2 = realloc(str, 1);
	// ft_putstr(str2);
	// ft_putchar('\n');
	// ft_printf("str1 address : %p\n", str1);
	// ft_printf("str2 address : %p\n", str2);
	// ft_putstr(str1);
	// ft_putchar('\n');
	// ft_printf("str1 content : %s\n", str1);
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
