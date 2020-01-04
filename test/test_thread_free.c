/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_thread_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 19:39:30 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/04 02:43:03 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void		*test_malloc_free(void *ptr)
{
	free(ptr);
	show_alloc_mem_free();
	return NULL;
}

int			main(void)
{
	void	*ptr[4];
	int		i;

	i = 0;
	pthread_t tid[4];
	ptr[0] = malloc(1);
	ptr[1] = malloc(1);
	ptr[2] = malloc(1);
	ptr[3] = malloc(1);
	while (i < 4)
	{
		pthread_create(&tid[i], NULL, test_malloc_free, ptr[i]);
		
		i++;
	}
	i = 0;
	while (i < 4)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
	show_alloc_mem_free();
	return (0);
}
