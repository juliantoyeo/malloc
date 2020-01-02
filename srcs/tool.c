/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyeo <jyeo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 21:22:09 by jyeo              #+#    #+#             */
/*   Updated: 2020/01/02 21:24:34 by jyeo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

size_t		ft_align_chunk(size_t len, size_t zone_size)
{
	if (zone_size == TINY_ZONE_SIZE)
	{
		if (len == 0)
			return (TINY_CHUNK_SIZE);
		else
			return ((((len - 1) >> 4) << 4) + TINY_CHUNK_SIZE);
	}
	else if (zone_size == SMALL_ZONE_SIZE)
		return ((((len - 1) >> 9) << 9) + SMALL_CHUNK_SIZE);
	else
		return ((((len - 1) >> 12) << 12) + PAGE_SIZE);
}

// t_zone		**ft_find_zone_realloc(size_t *size, void *ptr)
// {
// 	void	*tiny_start;
// 	void	*tiny_end;
// 	void	*small_start;
// 	void	*small_end;

// 	tiny_start = g_map.tiny;
// 	tiny_end = tiny_start + TINY_ZONE_SIZE - 1;
// 	small_start = g_map.small;
// 	small_end = small_start + SMALL_ZONE_SIZE - 1;
// 	if (tiny_start < ptr && ptr < tiny_end)
// 	{
// 		(*size) = TINY_ZONE_SIZE;
// 		return (&g_map.tiny);
// 	}
// 	else if (small_start < ptr && ptr < small_end)
// 	{
// 		(*size) = SMALL_ZONE_SIZE;
// 		return (&g_map.small);
// 	}
// 	return (NULL);
// }

t_zone		**ft_find_zone(size_t *size, void *ptr)
{
	void	*tiny_start;
	void	*tiny_end;
	void	*small_start;
	void	*small_end;

	tiny_start = g_map.tiny;
	tiny_end = tiny_start + TINY_ZONE_SIZE - 1;
	small_start = g_map.small;
	small_end = small_start + SMALL_ZONE_SIZE - 1;
	if (tiny_start < ptr && ptr < tiny_end)
	{
		(*size) = TINY_ZONE_SIZE;
		return (&g_map.tiny);
	}
	else if (small_start < ptr && ptr < small_end)
	{
		(*size) = SMALL_ZONE_SIZE;
		return (&g_map.small);
	}
	return (NULL);
}
