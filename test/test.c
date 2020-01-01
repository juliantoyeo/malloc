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

int main()
{
  // ft_printf("im here");
  // munmap(g_map.tiny, TINY_ZONE_SIZE);
  // show_alloc_mem();
  // munmap(g_map.small, SMALL_ZONE_SIZE);
  // char *p1 = malloc(1);
  // char *p2 = malloc(2);
  // free((void *)p1-32);
  // char *p2 = malloc(17);
  show_alloc_mem();
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
