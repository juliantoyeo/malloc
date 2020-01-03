/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_thread_realloc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 19:39:30 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/04 00:18:31 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void		*test_realloc(void *ptr)
{
	realloc(ptr, 1024);
	return NULL;
}

int			main(void)
{
	void	*ptr[4];
	int		i;

	i = 0;
	pthread_t tid[4];
	ptr[0] = malloc(4099);
	ptr[1] = malloc(4099);
	ptr[2] = malloc(4099);
	ptr[3] = malloc(4099);
	show_alloc_mem();
	while (i < 4)
	{
		pthread_create(&tid[i], NULL, test_realloc, ptr[i]);
		i++;
	}
	i = 0;
	while (i < 4)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
	show_alloc_mem();
	return (0);
}
