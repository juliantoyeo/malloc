/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/03 23:46:44 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#define M (1024 * 1024)

void print(char *s)
{
  write(1, s, strlen(s));
}

int main()
{
  char *addr1;
  char *addr3;

  addr1 = (char*)malloc(16*M);
  strcpy(addr1, "Bonjours\n");
  print(addr1);
  addr3 = (char*)realloc(addr1, 128*M);
  addr3[127*M] = 42;
  print(addr3);
	return (0);
};
