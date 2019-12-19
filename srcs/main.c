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

size_t   ft_align_chunk(size_t len, size_t zone_size)
{
	if (zone_size == TINY_ZONE_SIZE)
		return (((((len)-1)>>4)<<4)+16);
	else
		return (((((len)-1)>>9)<<9)+512);
};

t_block  *ft_create_block(t_zone *zone, size_t zone_size, size_t size, \
  t_block *prev_block)
{
	void    *new_location;
	t_block *block;

	new_location = (void *)prev_block + (prev_block->size_and_flag >> 1)
		+ BLOCK_SIZE;
	if (((((void *)zone + zone_size) - (void *)new_location)) \
		< (long)(BLOCK_SIZE + size))
	return NULL; // not enough size to be given at the new location
	block = new_location;
	block->size_and_flag = 1;
	block->next = NULL;
	return (block);
};

void    ft_create_zone(t_zone **zone, size_t zone_size)
{
	t_block *first_block;
	void    *page;

	page = mmap(0, zone_size, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0);
  // first_block = ft_create_block(page + sizeof(t_zone));
	first_block = page + sizeof(t_zone);
	first_block->size_and_flag = 1;
	first_block->next = NULL;
	(*zone) = page;
	(*zone)->remaining = zone_size - sizeof(t_zone) - sizeof(t_block);
	(*zone)->block = first_block;
};

void	ft_get_block(t_zone *zone, size_t zone_size, size_t size, \
	t_block **block)
{
	t_block *prev;

	prev = NULL;
	while ((*block) && ((*block)->size_and_flag & 1) != 1)
	{
		prev = (*block);
		(*block) = (*block)->next;
	};
	if((*block) == NULL)
	{
	(*block) = ft_create_block(zone, zone_size, size, prev);
		// return NULL;
	};
	if (prev)
		prev->next = (*block);
	// ft_printf("im prev block %p \n", prev);
	// ft_printf("im block %p \n", (*block));
	// ft_printf("im next block %p \n\n", (*block)->next);
	
};

void      *ft_alloc_data(t_zone **zone, size_t zone_size, size_t len)
{
	size_t  size;
	t_block *block;
	t_block *prev;

	if ((*zone) == NULL)
		ft_create_zone(zone, zone_size);
	block = (*zone)->block;
	prev = NULL;
	size = ft_align_chunk(len, zone_size);
	if ((*zone)->remaining < (size + BLOCK_SIZE))
    	return (NULL); // not enough size to be given
	ft_get_block((*zone), zone_size, size, &block);
	if(block == NULL)
	{
		ft_printf("im null\n");
		return (NULL);
	}
		
	ft_printf("im block %p \n\n", block);
	// while (block && (block->size_and_flag & 1) != 1)
	// {
	// 	prev = block;
	// 	block = block->next;
	// };
	// if(block == NULL)
	// {
	// 	if ((block = ft_create_block((*zone), zone_size, size, prev)) == NULL)
	// 		return NULL;
	// };
	// if (prev)
	// 	prev->next = block;
	block->size_and_flag = (size << 1);
	(*zone)->remaining -= size;
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

int main(int ac, char **av)
{
	ac = 0;
	av = NULL;
	// ft_malloc(ft_atoi(av[1]));

	// ft_malloc(4033);
	// ft_malloc(1);

	char *p1 = ft_malloc(1);
	// char *s1 = ft_malloc(497);
	char *p2 = ft_malloc(17);
	// char *s2 = ft_malloc(1024);
	// ft_strcpy(p1, "0123456789abcdef~!!");
	char *p3 = ft_malloc(10);
	// char *s3 = ft_malloc(512);
	ft_printf("p1 address : %p\n", p1);
	// ft_printf("s1 address : %p\n", s1);
	// ft_printf("p1 content : %s\n", p1);
	ft_printf("p2 address : %p\n", p2);
	// ft_printf("s2 address : %p\n", s2);
	ft_printf("p3 address : %p\n", p3);
	// ft_printf("s3 address : %p\n", s3);

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
