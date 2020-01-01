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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void print(char *s)
{
  write(1, s, strlen(s));
}

int main()
{
  char *addr;

  addr = malloc(16);
  free(NULL);
  free((void *)addr + 5);
  if (realloc((void *)addr + 5, 10) == NULL)
    print("Bonjours\n");
};
