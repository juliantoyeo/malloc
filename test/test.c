/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/04 00:01:23 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
  void *ptr1;
  void *ptr2;
  // void *ptr3;

  ptr1 = malloc(17);
  ptr2 = NULL;
  printf("%p\n", ptr1);
  ft_print_base((long)ptr1, 16);
  ft_putchar('\n');
  printf("%p\n", ptr2);
  ft_print_base((long)ptr2, 16);
  ft_putchar('\n');
  // ptr3 = realloc(ptr2, 17);
  // ft_printf("im here");
  // munmap(g_map.tiny, TINY_ZONE_SIZE);
  // show_alloc_mem();
  // munmap(g_map.small, SMALL_ZONE_SIZE);
  // char *p1 = malloc(1);
  // char *p2 = malloc(2);
  // free((void *)p1-32);
  // char *p2 = malloc(17);
  // show_alloc_mem();
  // printf("im p1 pointer %p\n", p1);
  // printf("im p1 pointer %s\n", p1);
  // printf("im p1 pointer %p\n", p1 - 32);
  // p1 = NULL;
  // p2 = NULL;
  // ft_free(p1);
  // show_alloc_mem();
  // ft_free(p2);
  // show_alloc_mem();
	return (0);
};
