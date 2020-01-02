/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_thread_malloc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 19:39:30 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/02 20:42:08 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void		*test_malloc(int i)
{
	void	*ptr;
	
	ptr = malloc(1);
	printf("Address off pointer %d : %p\n", i + 1, ptr);
	return NULL;
}

int			main(void)
{
	int		i;

	i = 0;
	pthread_t tid[2];
	while (i < 2)
	{
		pthread_create(&tid[i], NULL, test_malloc, i);
		i++;
	}
	pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	show_alloc_mem();
	return (0);
}
