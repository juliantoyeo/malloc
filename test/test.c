/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/05 21:04:54 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
  void *p1 = malloc(1);
  void *p2 = malloc(17);
  void *p3 = malloc(33);
  void *s1 = malloc(512);
  void *s2 = malloc(1024);
  void *s3 = malloc(1600);
  void *l1 = malloc(4097);
  void *l2 = malloc(8200);

  show_alloc_mem();
  free(p1);
  free(p2);
  free(p3);
  free(s1);
  free(s2);
  free(s3);
  free(l1);
  free(l2);
  // ptr = malloc(1024 * 1024 * 1024 * 10);
  
  // ptr = malloc(-1);
  // ptr = malloc(1024 * 1024 * 1024);
  // free(ptr);


  // // test error message
  // ptr = malloc(9223372036854770000); // large memory to allocate
  // ptr = NULL; // test invalide pointer
  // free(ptr);
  // // test error message

  show_alloc_mem();
  show_alloc_mem_free();
	return (0);
};
