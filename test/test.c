/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/04 02:37:40 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
  void *ptr;

  // ptr = malloc(1024 * 1024 * 1024 * 10);
  
  // ptr = malloc(-1);
  ptr = malloc(1024 * 1024 * 1024);
  // free(ptr);
  // ptr = malloc(1);
  // int i;

  // i = 0;
  // while (i < 2048)
  // {
  //   malloc(4098);
  //   i++;
  // }
  // // test error message
  // ptr = malloc(9223372036854770000); // large memory to allocate
  // ptr = NULL; // test invalide pointer
  // free(ptr);
  // // test error message
  show_alloc_mem();
  show_alloc_mem_free();
	return (0);
};
