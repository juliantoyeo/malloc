/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliantoyeo <juliantoyeo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 22:30:06 by juliantoyeo       #+#    #+#             */
/*   Updated: 2020/01/04 02:00:56 by juliantoyeo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_print_recursive(int64_t n, int base)
{
	char	c;
	int64_t	num;

	num = n;
    if (num == 0)
    {
        if (base == 16)
            write(1, "0x", 2);
        return ;
    }
    ft_print_recursive(num / base, base);
    if (num % base > 9)
        c = (((num % base) - 10) + 'a');
    else
        c = ((num % base) + '0');
    write(1, &c, 1);
}

void		ft_print_base(int64_t num, int base)
{
	ft_print_recursive(num, base);
	if (num == 0)
		write(1, "0", 1);
}
